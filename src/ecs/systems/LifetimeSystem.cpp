#include "LifetimeSystem.hpp"

#include "../../core/WorldContext.hpp"

void LifetimeSystem::update(WorldContext& worldContext, float deltaTime) {
	auto& lifetimes = worldContext.getEntityManager().getLifetimes();

	for (auto& [index, lifetime] : lifetimes.getAll()) {
		SDL_Log("Ageing the entity.");
		SDL_Log("remaining: %f", lifetime.remaining);
		if (!lifetime.isDead) {
			lifetime.remaining -= deltaTime;
			if (lifetime.remaining <= 0.0f) {
				SDL_Log("It's dead jim....");
				lifetime.isDead = true;
			}
		}
	}
}