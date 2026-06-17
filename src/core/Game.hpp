#pragma once

#include <vector>

#include "../platform/SDLManager.hpp"
#include "../ecs/entities/EntityManager.hpp"
#include "../ecs/systems/SystemManager.hpp"
#include "../input/InputManager.hpp"
#include "../events/EventManager.hpp"

#include "WorldContext.hpp"

class Game {
public:
	void initialize();
	void run();
	void shutdown();

	// will be moved after DrON
	void spawnAsteroid(Vec2 position, Vec2 velocity, AsteroidSize size);

private:
	WorldContext worldContext_;

	SDLManager SDLManager_;
	EntityManager entityManager_;
	SystemManager systemManager_;
	InputManager inputManager_;
	EventManager eventManager_;

	bool running_ = true;

	std::vector<SDL_Event> frameEvents_;
};