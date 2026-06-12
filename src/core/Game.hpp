#pragma once

#include <vector>

#include "../platform/SDLManager.hpp"
#include "../ecs/entities/EntityManager.hpp"
#include "../ecs/systems/SystemManager.hpp"
#include "WorldContext.hpp"

class Game {
public:
	void initialize();
	void run();
	void shutdown();

private:
	WorldContext worldContext_;

	SDLManager SDLManager_;
	EntityManager entityManager_;
	SystemManager systemManager_;

	bool running_ = true;

	std::vector<SDL_Event> frameEvents_;

	float rectX_ = 0.0f;
	float rectY_ = 90.f;
	float rectSpeed_ = 50.0f;
};