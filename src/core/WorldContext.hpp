#pragma once

#include <cassert>

#include "../ecs/entities/EntityManager.hpp"
#include "../platform/SDLManager.hpp"
#include "../ecs/systems/SystemManager.hpp"
#include "../input/InputManager.hpp"

class WorldContext {
public:
	// setters for them
	void setEntityManager(EntityManager* entityManager) {
		entityManagerPtr_ = entityManager;
	}

	void setSDLManager(SDLManager* SDL_Manager) {
		SDLManagerPtr_ = SDL_Manager;
	}

	void setSystemManager(SystemManager* systemManager) {
		systemManagerPtr_ = systemManager;
	}

	void setInputManager(InputManager* inputManager) {
		inputManagerPtr_ = inputManager;
	}

	// asserting reference acessor for each
	EntityManager& getEntityManager();
	SDLManager& getSDLManager();
	SystemManager& getSystemManager();
	InputManager& getInputManager();

	// isValid() to check nothing is null
	bool isValid() const;
	
private:
	// non-owning pointers to entity manager and sdl manager
	EntityManager* entityManagerPtr_ = nullptr;
	SDLManager* SDLManagerPtr_ = nullptr;
	SystemManager* systemManagerPtr_ = nullptr;
	InputManager* inputManagerPtr_ = nullptr;
};