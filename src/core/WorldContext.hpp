#pragma once

// The managers are referenced here only through pointers (members) and
// references (accessor return types), so forward declarations are sufficient.
// Translation units that actually call into a manager include its full
// definition directly.
class EntityManager;
class SDLManager;
class SystemManager;
class InputManager;
class EventManager;
class SceneManager;

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

	void setEventManager(EventManager* eventManager) {
		eventManagerPtr_ = eventManager;
	}

	void setSceneManager(SceneManager* sceneManager) {
		sceneManagerPtr_ = sceneManager;
	}

	// asserting reference acessor for each
	EntityManager& getEntityManager();
	SDLManager& getSDLManager();
	SystemManager& getSystemManager();
	InputManager& getInputManager();
	EventManager& getEventManager();
	SceneManager& getSceneManager();

	// isValid() to check nothing is null
	bool isValid() const;
	
private:
	// non-owning pointers to entity manager and sdl manager
	EntityManager* entityManagerPtr_ = nullptr;
	SDLManager* SDLManagerPtr_ = nullptr;
	SystemManager* systemManagerPtr_ = nullptr;
	InputManager* inputManagerPtr_ = nullptr;
	EventManager* eventManagerPtr_ = nullptr;
	SceneManager* sceneManagerPtr_ = nullptr;
};