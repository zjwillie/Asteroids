#include "pch.h"

#include "LifetimeSystem.hpp"

#include "../../core/WorldContext.hpp"
#include "../entities/EntityManager.hpp"
#include "../../utilities/logger/Logger.hpp"

void LifetimeSystem::update(WorldContext& worldContext, float deltaTime) {
	auto& lifetimes = worldContext.getEntityManager().getLifetimes();

	for (auto& [index, lifetime] : lifetimes.getAll()) {
        if (!lifetime.isDead && lifetime.remaining > 0.0f) {
            lifetime.remaining -= deltaTime;
            if (lifetime.remaining <= 0.0f) {
                lifetime.isDead = true;
            }
        }
	}
}