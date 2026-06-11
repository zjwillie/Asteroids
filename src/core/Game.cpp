#include "Game.hpp"

void Game::initialize() {
	sdlManager_.initialize("Asteroids", 1280, 720);
}

void Game::run() {
	while(running_){
		// if ESC, flip running to false
		if (!sdlManager_.pollEvents(frameEvents_)) { running_ = false; break; }


		float deltaTime = sdlManager_.getDeltaTime();

		sdlManager_.clear(20, 20, 30);
		sdlManager_.present();
	}
}

void Game::shutdown() {
	sdlManager_.shutdown();
}