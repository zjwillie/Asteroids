#include "LifetimeSystem.hpp"

#include "../../core/WorldContext.hpp"
#include "../../utilities/logger/Logger.hpp"

void LifetimeSystem::update(WorldContext& worldContext, float deltaTime) {
	auto& lifetimes = worldContext.getEntityManager().getLifetimes();

	for (auto& [index, lifetime] : lifetimes.getAll()) {
		//LOG_DEBUG("Lifetime", "Ageing the entity.");
		//LOG_DEBUG("Lifetime", "remaining: {}", lifetime.remaining);
		if (!lifetime.isDead) {
			lifetime.remaining -= deltaTime;
			if (lifetime.remaining <= 0.0f) {
				LOG_DEBUG("Lifetime", "Entity {} expired", index);
				lifetime.isDead = true;
			}
		}
	}
}