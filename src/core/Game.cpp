#include "Game.hpp"

void Game::initialize() {
	sdlManager_.initialize("Asteroids", 1280, 720);
}

void Game::run() {

	// set running flag
	running_ = true;

	while(running_){
		if (!sdlManager_.pollEvents(frameEvents_)) break;

		for (const SDL_Event& event : frameEvents_) {
			if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
				running_ = false;
			}
		}

		float deltaTime = sdlManager_.getDeltaTime();

		rectX_ += rectSpeed_ * deltaTime;
		if (rectX_ > 320.0f) rectX_ = 0.0f;

		SDL_FRect rect = { rectX_, rectY_, 10.0f, 10.0f };

		sdlManager_.clear(20, 20, 30);

		SDL_SetRenderDrawColor(sdlManager_.getRenderer(), 255, 255, 255, 255);
		SDL_RenderFillRect(sdlManager_.getRenderer(), &rect);

		sdlManager_.present();
	}
}

void Game::shutdown() {
	sdlManager_.shutdown();
}