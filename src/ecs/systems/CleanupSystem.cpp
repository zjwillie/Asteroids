#include <vector>

#include "CleanupSystem.hpp"

#include "../../core/WorldContext.hpp"

void CleanupSystem::update(WorldContext& worldContext, float deltaTime) {
	auto& lifetimes = worldContext.getEntityManager().getLifetimes();

	std::vector<EntityHandle> toDestroy{};

	for (auto& [index, lifetime] : lifetimes.getAll()) {
		if (lifetime.isDead) {
			EntityHandle handle = worldContext.getEntityManager().getHandle(index);
			toDestroy.push_back(handle);
		}
	}

	for (EntityHandle handle : toDestroy) {
		SDL_Log("Killing the entity.");
		worldContext.getEntityManager().destroy(handle);
	}
}