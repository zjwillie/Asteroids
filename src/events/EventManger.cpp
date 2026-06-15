#include <SDL3/SDL.h>

#include "EventManager.hpp"

void EventManager::emit(const FireRequestedEvent& fireRequest) {
	SDL_Log("WE HAVE BLASTERS! Pew pew... event");
	fireRequestedEvents_.push_back(fireRequest);
}

const std::vector<FireRequestedEvent>& EventManager::getFireRequests() {
	return fireRequestedEvents_;
}

void EventManager::clearFrame() {
	fireRequestedEvents_.clear();
}