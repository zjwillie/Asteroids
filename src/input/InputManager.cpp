// FUTURE: pressDurations ring buffer (parallel to pressTimestamps) could be added here.
// Fill on KEY_UP: pressDurations[totalPressCount % 16] = lastReleaseTime - lastPressTime
// This would enable combo/sequence detection in a higher-level InputInterpreter that
// reads patterns like: held A -> pressed B -> held A again, by walking the timestamp
// and duration history across multiple keys.
// InputManager stays pure data — interpretation lives in a separate system.

#include "InputManager.hpp"

float InputManager::getLastPressDuration(SDL_Scancode key) const {
	// make sure in bounds
	if (key >= SDL_SCANCODE_COUNT) return 0.0f;
	// make sure the key has been pressed at least once
	if (keyboardState_[key].totalPressCount == 0) return 0.0f;
	// if the key is currently down, return the current time
	if (keyboardState_[key].isDown) {
		return (SDL_GetTicks() / 1000.0f) - keyboardState_[key].lastPressTime;
	}
	// finally just give the data
	return keyboardState_[key].lastReleaseTime - keyboardState_[key].lastPressTime;
}

void InputManager::processEvent(const SDL_Event& event) {
	if (event.type == SDL_EVENT_KEY_DOWN) {
		// store the key for convience below
		SDL_Scancode key = event.key.scancode;
		
		if (!keyboardState_[key].isDown) {
			float pressTime = SDL_GetTicks() / 1000.0f;

			// KEY_DOWN block, after updating state:
			SDL_Log("KEY DOWN: %d", key);

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
		float pressTime = SDL_GetTicks() / 1000.0f;

		// KEY_UP block, after updating state:
		SDL_Log("KEY UP: %d", key);

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
	if (key < SDL_SCANCODE_COUNT) {
		return keyboardState_[key].isDown;
	}
	return false;
}

bool InputManager::wasPressed(SDL_Scancode key) const {
	if (key < SDL_SCANCODE_COUNT) {
		return keyboardState_[key].wasPressed;
	}
	return false;
}

bool InputManager::wasReleased(SDL_Scancode key) const {
	if (key >= 0 && key < SDL_SCANCODE_COUNT) {
		return keyboardState_[key].wasReleased;
	}
	return false;
}

void InputManager::beginFrame() {
	for (SDL_Scancode key : dirtyKeys_) {
		if (key == SDL_SCANCODE_SPACE) SDL_Log("Clearing space wasPressed");

		keyboardState_[key].wasPressed = false;
		keyboardState_[key].wasReleased = false;
	}
	dirtyKeys_.clear();
}