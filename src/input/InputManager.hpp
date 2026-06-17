#pragma once

#include <array>
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
	std::array<KeyState, SDL_SCANCODE_COUNT> keyboardState_{};
	std::vector<SDL_Scancode> dirtyKeys_{};
};