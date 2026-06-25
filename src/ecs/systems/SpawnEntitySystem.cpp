#include "pch.h"

#include "SpawnEntitySystem.hpp"

#include "../../core/WorldContext.hpp"
#include "../entities/EntityManager.hpp"
#include "../../events/EventManager.hpp"
#include "../../events/SpawnEntityEvent.hpp"

//#include "Logger.hpp"


void SpawnEntitySystem::update(WorldContext& worldContext, float deltaTime) {
	const auto& spawnEntityEvents = worldContext.getEventManager().getSpawnRequests();
}