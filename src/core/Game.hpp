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
};