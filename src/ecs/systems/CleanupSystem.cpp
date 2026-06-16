#include <vector>

#include "CleanupSystem.hpp"

#include "../../core/WorldContext.hpp"
#include "../../utilities/logger/Logger.hpp"


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
		LOG_DEBUG("Cleanup", "Destroying entity {} (gen {})", handle.index, handle.generation);
		worldContext.getEntityManager().destroy(handle);
	}
}