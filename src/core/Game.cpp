#include "Game.hpp"

#include "../ecs/systems/MovementSystem.hpp"
#include "../ecs/systems/RenderSystem.hpp"

#include <memory>

void Game::initialize() {
	SDLManager_.initialize("Asteroids", 1280, 720);
	worldContext_.setEntityManager(&entityManager_);
	worldContext_.setSDLManager(&SDLManager_);
	worldContext_.setSystemManager(&systemManager_);

	if (!worldContext_.isValid()) {
		// fuck me maybe we should have setting running_ = true here after all, nice way to bail yeah?
		// so I changed the defalut to running_ = true until proven otherwise
		running_ = false;
		// logger soon
		return;
	}

	systemManager_.registerSystem(std::make_unique<MovementSystem>());
	systemManager_.registerSystem(std::make_unique<RenderSystem>());
}

void Game::run() {
	if (!running_) return;

	while(running_){
		if (!SDLManager_.pollEvents(frameEvents_)) break;

		for (const SDL_Event& event : frameEvents_) {
			if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
				running_ = false;
			}
		}

		// check your watches
		float deltaTime = SDLManager_.getDeltaTime();

		// move the rect, keep for now so we can watch it along side the rest
		rectX_ += rectSpeed_ * deltaTime;
		if (rectX_ > 320.0f) rectX_ = 0.0f;

		// update
		systemManager_.update(worldContext_, deltaTime);

		// clear the screen
		SDLManager_.clear(20, 20, 30);

		// draw
		SDL_FRect rect = { rectX_, rectY_, 10.0f, 10.0f };
		systemManager_.render(worldContext_);

		SDL_SetRenderDrawColor(SDLManager_.getRenderer(), 255, 255, 255, 255);
		SDL_RenderFillRect(SDLManager_.getRenderer(), &rect);

		SDLManager_.present();
	}
}

void Game::shutdown() {
	SDLManager_.shutdown();
}