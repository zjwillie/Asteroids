#include "pch.h"

#include "SystemManager.hpp"

void SystemManager::registerSystem(std::unique_ptr<System> system) {
	// don't break the law, only move unique_ptrs
	systems_.push_back(std::move(system));
}

void SystemManager::update(WorldContext& worldContext, float deltaTime) {
	for (auto& system : systems_) {
		system->update(worldContext, deltaTime);
	}
}

void SystemManager::render(WorldContext& worldContext) {
	for (auto& system : systems_) {
		system->render(worldContext);
	}
}