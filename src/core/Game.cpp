#include "pch.h"

#include <memory>

#include "Game.hpp"

#include "../ecs/systems/MovementSystem.hpp"
#include "../ecs/systems/RenderSystem.hpp"
#include "../ecs/systems/ShipControlSystem.hpp"
#include "../ecs/systems/WarpSystem.hpp"
#include "../ecs/systems/LifetimeSystem.hpp"
#include "../ecs/systems/CleanupSystem.hpp"
#include "../ecs/systems/WeaponSystem.hpp"
#include "../ecs/systems/CollisionSystem.hpp"
#include "../ecs/systems/AsteroidResponseSystem.hpp"
#include "../ecs/systems/ShipResponseSystem.hpp"
#include "../ecs/systems/SpawnEntitySystem.hpp"

// Need while spawning asteroids in init
#include "../ecs/components/Asteroid.hpp"

#include "Logger.hpp"
#include "FileSink.hpp"
#include "../utilities/random/Random.hpp"

#include <cmath>

namespace {
	// Axis-aligned rectangle [x, y, w, h], as authored in a level's
	// distribution / exclusion lists.
	struct Rect { float x, y, w, h; };

	// Read a DrON list-of-[x, y, w, h] into Rects. Missing entries read as 0.
	std::vector<Rect> readRects(const DronValue& list) {
		std::vector<Rect> rects;
		for (auto&& r : list.elements()) {
			rects.push_back(Rect{
				r[static_cast<std::size_t>(0)].get_or(0.0f),
				r[static_cast<std::size_t>(1)].get_or(0.0f),
				r[static_cast<std::size_t>(2)].get_or(0.0f),
				r[static_cast<std::size_t>(3)].get_or(0.0f),
			});
		}
		return rects;
	}

	bool contains(const Rect& r, float x, float y) {
		return x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h;
	}

	// Uniformly sample a point inside one of the distribution rects, rejecting
	// points that land in any exclusion rect. Falls back to a distribution
	// centre if no free spot is found within the attempt budget.
	Vec2 samplePosition(const std::vector<Rect>& distribution, const std::vector<Rect>& exclusion) {
		if (distribution.empty()) return Vec2{ 0.0f, 0.0f };
		for (int attempt = 0; attempt < 32; ++attempt) {
			const Rect& r = distribution[Random::intRange(0, static_cast<int>(distribution.size()) - 1)];
			float x = Random::floatRange(r.x, r.x + r.w);
			float y = Random::floatRange(r.y, r.y + r.h);
			bool blocked = false;
			for (const Rect& e : exclusion) if (contains(e, x, y)) { blocked = true; break; }
			if (!blocked) return Vec2{ x, y };
		}
		const Rect& r = distribution[0];
		return Vec2{ r.x + r.w * 0.5f, r.y + r.h * 0.5f };
	}
}

void Game::initialize() {
	Logger::initialize();
	Logger::addSink(std::make_unique<FileSink>("game.log"));

	LOG_INFO("Game", "Logger online");

	if (!SDLManager_.initialize("Asteroids", 1280, 720)) {
		LOG_FATAL("SDL", "Failed to initialize SDL");
		running_ = false;
		return;
	}

	worldContext_.setEntityManager(&entityManager_);
	worldContext_.setSDLManager(&SDLManager_);
	worldContext_.setSystemManager(&systemManager_);
	worldContext_.setInputManager(&inputManager_);
	worldContext_.setEventManager(&eventManager_);
	worldContext_.setSceneManager(&sceneManager_);

	if (!worldContext_.isValid()) {
		// fuck me maybe we should have setting running_ = true here after all, nice way to bail yeah?
		// so I changed the defalut to running_ = true until proven otherwise
		running_ = false;
		LOG_INFO("Game", "WorldContext is validated.");
		return;
	}

	Random::initialize();

	sceneManager_.loadScene("assets/scenes/game.scene");

	// emit scene entities (persistent — player, UI). Every named-entity spawn
	// passes through applyOverrides so level-specific overrides are always applied.
	for (auto&& [name, path] : sceneManager_.sceneData.entities.items()) {
		DronDocument doc = DronConfig::load(path.as<std::string>());
		sceneManager_.applyOverrides(name, doc);
		SpawnEntityEvent event{};
		event.entity = std::move(doc);
		eventManager_.emit(event);
	}

	// emit fixed level entities (any explicitly listed in [Entities])
	for (auto&& [name, path] : sceneManager_.levelData.entities.items()) {
		DronDocument doc = DronConfig::load(path.as<std::string>());
		sceneManager_.applyOverrides(name, doc);
		SpawnEntityEvent event{};
		event.entity = std::move(doc);
		eventManager_.emit(event);
	}

	// spawn the level's asteroid field: `number` large asteroids at random
	// positions inside `distribution` (avoiding `exclusion`), each with a random
	// heading and a speed within `speed`. Built from the cached LARGE archetype,
	// stamped, and routed through the same SpawnEntityEvent -> factory pipeline.
	{
		const DronValue& config = sceneManager_.levelData.asteroids;
		int count = config["number"].get_or(0);
		std::vector<Rect> distribution = readRects(config["distribution"]);
		std::vector<Rect> exclusion = readRects(config["exclusion"]);
		float speedMin = config["speed"][static_cast<std::size_t>(0)].get_or(20.0f);
		float speedMax = config["speed"][static_cast<std::size_t>(1)].get_or(50.0f);

		for (int i = 0; i < count; ++i) {
			Vec2 position = samplePosition(distribution, exclusion);
			float heading = Random::floatRange(0.0f, 2.0f * 3.14159265f);
			float speed = Random::floatRange(speedMin, speedMax);
			Vec2 velocity = Vec2{ std::cos(heading), std::sin(heading) } * speed;

			DronWriter blueprint{ sceneManager_.getAsteroidArchetype("LARGE") };
			blueprint["Transform"]["x"] = static_cast<double>(position.x);
			blueprint["Transform"]["y"] = static_cast<double>(position.y);
			blueprint["Velocity"]["dx"] = static_cast<double>(velocity.x);
			blueprint["Velocity"]["dy"] = static_cast<double>(velocity.y);

			SpawnEntityEvent event{};
			event.entity = blueprint.document();
			eventManager_.emit(event);
		}
	}

	
	// SpawnEntitySystem runs AFTER any system that can emit a SpawnEntityEvent
	// (e.g. AsteroidResponseSystem's splits), because EventManager::clearFrame()
	// wipes the queue every frame — a request must be consumed the same frame it
	// is emitted. Initial scene/level spawns (emitted before the loop) are still
	// picked up on frame 1.
	systemManager_.registerSystem(std::make_unique<ShipControlSystem>());
	systemManager_.registerSystem(std::make_unique<MovementSystem>());
	systemManager_.registerSystem(std::make_unique<WarpSystem>());
	systemManager_.registerSystem(std::make_unique<LifetimeSystem>());
	systemManager_.registerSystem(std::make_unique<CollisionSystem>());
	systemManager_.registerSystem(std::make_unique<AsteroidResponseSystem>());
	systemManager_.registerSystem(std::make_unique<SpawnEntitySystem>());
	systemManager_.registerSystem(std::make_unique<ShipResponseSystem>());
	systemManager_.registerSystem(std::make_unique<WeaponSystem>());
	systemManager_.registerSystem(std::make_unique<CleanupSystem>());
	systemManager_.registerSystem(std::make_unique<RenderSystem>());
}

void Game::run() {
	if (!running_) return;

	while(running_){
		inputManager_.beginFrame();

		if (!SDLManager_.pollEvents(frameEvents_)) break;

		for (const SDL_Event& event : frameEvents_) {
			if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
				running_ = false;
			}
			inputManager_.processEvent(event);
		}

		// check your watches
		float deltaTime = SDLManager_.getDeltaTime();

		// update
		systemManager_.update(worldContext_, deltaTime);

		// clear the screen
		SDLManager_.clear(20, 20, 30);

		// draw
		systemManager_.render(worldContext_);

		SDLManager_.present();

		eventManager_.clearFrame();
	}
}

void Game::shutdown() {
	LOG_INFO("Game", "Shutting down");
	SDLManager_.shutdown();
	Logger::shutdown();
}