#include "pch.h"

#include "WorldContext.hpp"

#include <cassert>

bool WorldContext::isValid() const {
	return entityManagerPtr_ != nullptr 
		&& SDLManagerPtr_ != nullptr
		&& systemManagerPtr_ != nullptr
		&& inputManagerPtr_ != nullptr
		&& eventManagerPtr_ != nullptr;
}

EntityManager& WorldContext::getEntityManager() {
	assert(entityManagerPtr_ && "EntityManager not initialized!");
	return *entityManagerPtr_;
}

SDLManager& WorldContext::getSDLManager() {
	assert(SDLManagerPtr_ && "SDLManager not initialized!");
	return *SDLManagerPtr_;
}

SystemManager& WorldContext::getSystemManager() {
	assert(systemManagerPtr_ && "SystemManager not initialized!");
	return *systemManagerPtr_;
}

InputManager& WorldContext::getInputManager() {
	assert(inputManagerPtr_ && "InputManager not initialized!");
	return *inputManagerPtr_;
}

EventManager& WorldContext::getEventManager() {
	assert(eventManagerPtr_ && "EventManager not initialized!");
	return *eventManagerPtr_;
}