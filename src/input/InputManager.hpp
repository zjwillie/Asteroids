#pragma once

#include <unordered_map>
#include <vector>

#include <SDL3/SDL.h>

#include "KeyState.hpp"

class InputManager {
public:
	void beginFrame();
	void processEvent(const SDL_Event& event);

	bool isDown(SDL_Scancode key) const;
	bool wasPressed(SDL_Scancode key) const;
	bool wasReleased(SDL_Scancode key) const;

	double getLastPressDuration(SDL_Scancode key) const;
private:
	std::unordered_map<SDL_Scancode, KeyState> keyboardState_{};
	std::vector<SDL_Scancode> dirtyKeys_{};
};