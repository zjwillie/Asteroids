#include "CollisionSystem.hpp"

#include "../../core/WorldContext.hpp"
#include "../../utilities/logger/Logger.hpp"

void CollisionSystem::update(WorldContext& worldContext, float deltaTime) {
	auto& colliders = worldContext.getEntityManager().getColliders();
	auto& transforms = worldContext.getEntityManager().getTransforms();

	for (auto& [indexA, colliderA] : colliders.getAll()) {
		for (auto& [indexB, colliderB] : colliders.getAll()) {
			// skip duplicates
			if (indexB <= indexA) continue;
			
			// check if they overlap, so we need their distance from one another and if that distance is less than the sum of their radiuss radii? then they are hitting each other, reasonable for asteroids I guess
			EntityHandle handleA = worldContext.getEntityManager().getHandle(indexA); // might could make a shortner for this in world context??
			EntityHandle handleB = worldContext.getEntityManager().getHandle(indexB); // might could make a shortner for this in world context??
			Transform* transformA = transforms.get(handleA);
			Transform* transformB = transforms.get(handleB);

			if (!transformA || !transformB) continue;

			float radius = colliderA.circle.radius + colliderB.circle.radius;
			Vec2 delta = transformA->position - transformB->position;
			float distSquared = delta.x * delta.x + delta.y * delta.y;

			// distance ^ 2 < radius * radius fast and dirty?
			if (distSquared < radius * radius) {
				bool aIsBullet		= worldContext.getEntityManager().getBullets().has(handleA);
				bool aIsAsteroid	= worldContext.getEntityManager().getAsteroids().has(handleA);
				bool aIsShip		= worldContext.getEntityManager().getPlayerControlled().has(handleA);
				bool bIsBullet		= worldContext.getEntityManager().getBullets().has(handleB);
				bool bIsAsteroid	= worldContext.getEntityManager().getAsteroids().has(handleB);
				bool bIsShip		= worldContext.getEntityManager().getPlayerControlled().has(handleB);
				if ((aIsBullet && bIsAsteroid) || (bIsBullet && aIsAsteroid)) {
					if constexpr (kDebugLogging) {
						LOG_DEBUG("Collision", "Bullet hit asteroid!");
					}
					EntityHandle bullet = aIsBullet ? handleA : handleB;
					EntityHandle asteroid = aIsAsteroid ? handleA : handleB;
					worldContext.getEventManager().emit(BulletHitAsteroidEvent{ bullet, asteroid });
				}

				if ((aIsShip && bIsAsteroid) || (bIsShip && aIsAsteroid)) {
					if constexpr (kDebugLogging) {
						LOG_DEBUG("Collision", "Ship hit asteroid!");
					}
					EntityHandle ship = aIsShip ? handleA : handleB;
					EntityHandle asteroid = aIsAsteroid ? handleA : handleB;
					worldContext.getEventManager().emit(ShipHitAsteroidEvent{ ship, asteroid });
				}
			}
		}
	}
}
