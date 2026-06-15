#include "Game.hpp"

#include "../ecs/systems/MovementSystem.hpp"
#include "../ecs/systems/RenderSystem.hpp"
#include "../ecs/systems/ShipControlSystem.hpp"
#include "../ecs/systems/WarpSystem.hpp"
#include "../ecs/systems/LifetimeSystem.hpp"
#include "../ecs/systems/CleanupSystem.hpp"
#include "../ecs/systems/WeaponSystem.hpp"

#include <memory>

void Game::initialize() {
	SDLManager_.initialize("Asteroids", 1280, 720);
	worldContext_.setEntityManager(&entityManager_);
	worldContext_.setSDLManager(&SDLManager_);
	worldContext_.setSystemManager(&systemManager_);
	worldContext_.setInputManager(&inputManager_);
	worldContext_.setEventManager(&eventManager_);

	if (!worldContext_.isValid()) {
		// fuck me maybe we should have setting running_ = true here after all, nice way to bail yeah?
		// so I changed the defalut to running_ = true until proven otherwise
		running_ = false;
		// logger soon
		return;
	}

	// set systems, this will be the order they are called so be careful
	systemManager_.registerSystem(std::make_unique<ShipControlSystem>());
	systemManager_.registerSystem(std::make_unique<MovementSystem>());
	systemManager_.registerSystem(std::make_unique<WarpSystem>());
	systemManager_.registerSystem(std::make_unique<LifetimeSystem>());
	systemManager_.registerSystem(std::make_unique<WeaponSystem>());
	systemManager_.registerSystem(std::make_unique<CleanupSystem>());
	systemManager_.registerSystem(std::make_unique<RenderSystem>());

	EntityHandle e1 = entityManager_.create();
	entityManager_.getTransforms().add(e1, Transform{ Vec2{ 20.0f, 30.0f }, 0.0f });
	entityManager_.getVelocities().add(e1, Velocity{ Vec2{ 0.0f, 0.0f }, 0.0f });
	entityManager_.getSprites().add(e1, Sprite{ 10, 10, 255, 100, 100 });
	entityManager_.getPlayerControlled().add(e1, PlayerControlled{});

	EntityHandle e2 = entityManager_.create();
	entityManager_.getTransforms().add(e2, Transform{ Vec2{ 80.0f, 60.0f }, 0.0f });
	entityManager_.getVelocities().add(e2, Velocity{ Vec2{ -3.0f, 5.0f }, 0.0f });
	entityManager_.getSprites().add(e2, Sprite{ 10, 10, 100, 255, 100 });
	entityManager_.getLifetimes().add(e2, Lifetime{ 10.0, false });

	EntityHandle e3 = entityManager_.create();
	entityManager_.getTransforms().add(e3, Transform{ Vec2{ 160.0f, 90.0f }, 0.0f });
	entityManager_.getVelocities().add(e3, Velocity{ Vec2{ 60.0f, -40.0f }, 0.0f });
	entityManager_.getSprites().add(e3, Sprite{ 10, 10, 100, 100, 255 });

	EntityHandle e4 = entityManager_.create();
	entityManager_.getTransforms().add(e4, Transform{ Vec2{ 240.0f, 120.0f }, 0.0f });
	entityManager_.getVelocities().add(e4, Velocity{ Vec2{ -50.0f, -30.0f }, 0.0f });
	entityManager_.getSprites().add(e4, Sprite{ 10, 10, 255, 255, 100 });

	EntityHandle e5 = entityManager_.create();
	entityManager_.getTransforms().add(e5, Transform{ Vec2{ 60.0f, 150.0f }, 0.0f });
	entityManager_.getVelocities().add(e5, Velocity{ Vec2{ 70.0f, 60.0f }, 0.0f });
	entityManager_.getSprites().add(e5, Sprite{ 10, 10, 255, 100, 255 });
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
	SDLManager_.shutdown();
}