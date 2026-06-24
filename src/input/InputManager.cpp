#include "pch.h"

// FUTURE: pressDurations ring buffer (parallel to pressTimestamps) could be added here.
// Fill on KEY_UP: pressDurations[totalPressCount % 16] = lastReleaseTime - lastPressTime
// This would enable combo/sequence detection in a higher-level InputInterpreter that
// reads patterns like: held A -> pressed B -> held A again, by walking the timestamp
// and duration history across multiple keys.
// InputManager stays pure data — interpretation lives in a separate system.

#include "InputManager.hpp"

#include "Logger.hpp"

double InputManager::getLastPressDuration(SDL_Scancode key) const {
	auto it = keyboardState_.find(key);
	if (it == keyboardState_.end() || it->second.totalPressCount == 0) return 0.0;
	if (it->second.isDown) {
		return (SDL_GetTicks() / 1000.0) - it->second.lastPressTime;
	}
	return it->second.lastReleaseTime - it->second.lastPressTime;
}

void InputManager::processEvent(const SDL_Event& event) {
	if (event.type == SDL_EVENT_KEY_DOWN) {
		// store the key for convience below
		SDL_Scancode key = event.key.scancode;
		
		if (!keyboardState_[key].isDown) {
			double pressTime = SDL_GetTicks() / 1000.0;

			// KEY_DOWN block, after updating state:
			LOG_DEBUG("Input", "KEY DOWN: {}", static_cast<int>(key));

			// update the key
			keyboardState_[key].wasPressed = true;
			keyboardState_[key].isDown = true;

			// add the key to the array to keep track
			keyboardState_[key].pressTimestamps[keyboardState_[key].totalPressCount % 16] = pressTime;
			++keyboardState_[key].totalPressCount;

			// update the last time pressed
			keyboardState_[key].lastPressTime = pressTime;

			// someone touched our clean key...
			dirtyKeys_.push_back(key);
		}
	}
	else if (event.type == SDL_EVENT_KEY_UP) {
		// convienice variable
		SDL_Scancode key = event.key.scancode;
		double pressTime = SDL_GetTicks() / 1000.0;

		// KEY_UP block, after updating state:
		LOG_DEBUG("Input", "KEY UP: {}", static_cast<int>(key));

		// update state
		keyboardState_[key].wasReleased = true;
		keyboardState_[key].isDown = false;

		// store that data
		keyboardState_[key].lastReleaseTime = pressTime;
		
		// key needs cleaning
		dirtyKeys_.push_back(key);
	}
}

bool InputManager::isDown(SDL_Scancode key) const {
	auto it = keyboardState_.find(key);
	return it != keyboardState_.end() && it->second.isDown;
}

bool InputManager::wasPressed(SDL_Scancode key) const {
	auto it = keyboardState_.find(key);
	return it != keyboardState_.end() && it->second.wasPressed;
}

bool InputManager::wasReleased(SDL_Scancode key) const {
	auto it = keyboardState_.find(key);
	return it != keyboardState_.end() && it->second.wasReleased;
}
void InputManager::beginFrame() {
	for (SDL_Scancode key : dirtyKeys_) {
		keyboardState_[key].wasPressed = false;
		keyboardState_[key].wasReleased = false;
	}
	dirtyKeys_.clear();
}