#include "EventManager.hpp"

void EventManager::emit(const FireRequestedEvent& fireRequest) {
	fireRequestedEvents_.push_back(fireRequest);
}

const std::vector<FireRequestedEvent>& EventManager::getFireRequests() {
	return fireRequestedEvents_;
}

void EventManager::clearFrame() {
	fireRequestedEvents_.clear();
}