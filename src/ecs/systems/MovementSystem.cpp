#include "pch.h"

#include "MovementSystem.hpp"

#include "../../core/WorldContext.hpp"
#include "../entities/EntityManager.hpp"
#include "../entities/EntityHandle.hpp"
#include "../components/Transform.hpp"

void MovementSystem::update(WorldContext& worldContext, float deltaTime) {
	auto& transforms = worldContext.getEntityManager().getTransforms();
	auto& velocities = worldContext.getEntityManager().getVelocities();

	for (auto& [index, velocity] : velocities.getAll()) {
		// Now we have access to both velocity and transform for an indexed entity
		// Make sure the entity is valid? or must it already be to be pulled here, yes I think so
		// grab the transfrom.x and y and add the velocities to them fin
		EntityHandle handle = worldContext.getEntityManager().getHandle(index);
		Transform* transform = transforms.get(handle);

		if (transform) {
			transform->position += velocity.linearVelocity * deltaTime;
			transform->rotation += velocity.angularVelocity * deltaTime;
		}
	}
}