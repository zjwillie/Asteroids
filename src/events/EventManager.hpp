#pragma once

#include <vector>

#include "FireRequestedEvent.hpp"

class EventManager {
public:
	void emit(const FireRequestedEvent& fireRequest);
	const std::vector<FireRequestedEvent>& getFireRequests();
	void clearFrame();

private:
	std::vector<FireRequestedEvent> fireRequestedEvents_;
};

