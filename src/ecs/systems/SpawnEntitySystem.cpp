#include "pch.h"

#include "SpawnEntitySystem.hpp"

#include "../../core/WorldContext.hpp"
#include "../entities/EntityManager.hpp"
#include "../../events/EventManager.hpp"
#include "../../events/SpawnEntityEvent.hpp"

// Fully generic: for each spawn request, create one entity, then walk the
// request document's sections. Each section name is looked up in the factory;
// if a loader exists it attaches that component. No entity-type switching —
// [Meta], [Physics] and any other unregistered section are skipped.
void SpawnEntitySystem::update(WorldContext& worldContext, float deltaTime) {
	EntityManager& entityManager = worldContext.getEntityManager();
	const auto& spawnRequests = worldContext.getEventManager().getSpawnRequests();

	for (const SpawnEntityEvent& spawnEvent : spawnRequests) {
		EntityHandle handle = entityManager.create();

		for (const auto& [section, node] : spawnEvent.entity) {
			if (!node.isMap()) continue;
			factory_.load(section, entityManager, handle, node.as<DronMap>());
		}
	}
}
