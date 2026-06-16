#include <SDL3/SDL.h>

#include "EventManager.hpp"
#include "../utilities/logger/Logger.hpp"

void EventManager::emit(const FireRequestedEvent& fireRequest) {
	LOG_INFO("Event", "WE HAVE BLASTERS! Pew pew... event");
	fireRequestedEvents_.push_back(fireRequest);
}

const std::vector<FireRequestedEvent>& EventManager::getFireRequests() {
	return fireRequestedEvents_;
}

void EventManager::clearFrame() {
	fireRequestedEvents_.clear();
}