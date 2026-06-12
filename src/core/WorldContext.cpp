#include "WorldContext.hpp"

bool WorldContext::isValid() const {
	return entityManagerPtr_ != nullptr 
		&& SDLManagerPtr_ != nullptr
		&& systemManagerPtr_ != nullptr;
}

EntityManager& WorldContext::getEntityManager() {
	assert(entityManagerPtr_ && "EntityManger not initialized!");
	return *entityManagerPtr_;
}

SDLManager& WorldContext::getSDLManager() {
	assert(SDLManagerPtr_ && "SDLManger not initialized!");
	return *SDLManagerPtr_;
}

SystemManager& WorldContext::getSystemManager() {
	assert(systemManagerPtr_ && "SystemManger not initialized!");
	return *systemManagerPtr_;
}