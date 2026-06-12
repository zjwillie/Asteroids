#pragma once

#include <cassert>

#include "ecs/entities/EntityManager.hpp"
#include "../platform/SDLManager.hpp"

class WorldContext {
public:
	// setters for them
	void setEntityManager(EntityManager* entityManager) {
		entityManagerPtr_ = entityManager;
	}

	void setSDLManager(SDLManager* SDL_Manager) {
		SDLManagerPtr_ = SDL_Manager;
	}

	// asserting reference acessor for each
	EntityManager& getEntityManager();
	SDLManager& getSDLManager();

	// isValid() to check nothing is null
	bool isValid() const;
	
private:
	// non-owning pointers to entity manager and sdl manager
	EntityManager* entityManagerPtr_ = nullptr;
	SDLManager* SDLManagerPtr_ = nullptr;
};

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