#pragma once

#include "WorldContext.hpp"

bool WorldContext::isValid() const {
	return entityManagerPtr_ != nullptr && SDLManagerPtr_ != nullptr;
}

EntityManager& WorldContext::getEntityManager() {
	assert(entityManagerPtr_ && "EntityManger not initialized!");
	return *entityManagerPtr_;
}

SDLManager& WorldContext::getSDLManager() {
	assert(SDLManagerPtr_ && "SDLManger not initialized!");
	return *SDLManagerPtr_;
}