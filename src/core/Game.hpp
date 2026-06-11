#pragma once

#include <vector>

#include "../platform/SDLManager.hpp"

class Game {
public:
	void initialize();
	void run();
	void shutdown();

private:
	SDLManager sdlManager_;
	bool running_ = false;
	std::vector<SDL_Event> frameEvents_;

	float rectX_ = 0.0f;
	float rectY_ = 90.f;
	float rectSpeed_ = 50.0f;
};