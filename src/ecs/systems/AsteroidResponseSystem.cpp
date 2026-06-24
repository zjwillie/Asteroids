#include "pch.h"

#include <vector>
#include <cmath>

#include "AsteroidResponseSystem.hpp"

#include "../../core/WorldContext.hpp"
#include "../entities/EntityManager.hpp"
#include "../../events/EventManager.hpp"
#include "Logger.hpp"
#include "../../utilities/random/Random.hpp"

void AsteroidResponseSystem::update(WorldContext& worldContext, float deltaTime) {
    const auto& bulletHitAsteroidEvents = worldContext.getEventManager().getBulletHitAsteroidEvents();

    for (const BulletHitAsteroidEvent bulletHitAsteroid : bulletHitAsteroidEvents) {
        if (!worldContext.getEntityManager().isAlive(bulletHitAsteroid.bulletHandle)) continue;
        if (!worldContext.getEntityManager().isAlive(bulletHitAsteroid.asteroidHandle)) continue;

        Name* asteroidName = worldContext.getEntityManager().getNames().get(bulletHitAsteroid.asteroidHandle);
        LOG_DEBUG("AsteroidResponseSystem", "Destroying {} ({}) '{}'",
            bulletHitAsteroid.asteroidHandle.index,
            bulletHitAsteroid.asteroidHandle.generation,
            asteroidName ? asteroidName->name : "unnamed"
        );

        // split logic
        Asteroid* asteroid = worldContext.getEntityManager().getAsteroids().get(bulletHitAsteroid.asteroidHandle);
        Transform* asteroidTransform = worldContext.getEntityManager().getTransforms().get(bulletHitAsteroid.asteroidHandle);
        Velocity* asteroidVelocity = worldContext.getEntityManager().getVelocities().get(bulletHitAsteroid.asteroidHandle);

        if (asteroid && asteroidTransform && asteroidVelocity && asteroid->size != AsteroidSize::Small) {
            AsteroidSize smallerSize = static_cast<AsteroidSize>(static_cast<int>(asteroid->size) - 1);
            int spawnCount = (asteroid->size == AsteroidSize::Medium) ? 4 : 2;

            float baseSpeed = asteroidVelocity->linearVelocity.length() * 1.3f;
            Vec2 direction = asteroidVelocity->linearVelocity.normalized();

            float spreadAngle = (2.0f * 3.14159f) / spawnCount;

            for (int i = 0; i < spawnCount; ++i) {
                float angle = spreadAngle * i + Random::floatRange(-5.5f, 5.5f);
                float speed = baseSpeed * Random::floatRange(0.8f, 2.3f);

                Vec2 spawnVelocity = Vec2{
                    direction.x * std::cos(angle) - direction.y * std::sin(angle),
                    direction.x * std::sin(angle) + direction.y * std::cos(angle)
                } * speed;

                spawnAsteroid(worldContext, asteroidTransform->position, spawnVelocity, smallerSize);
            }
        }

        Lifetime* bulletLifetime = worldContext.getEntityManager().getLifetimes().get(bulletHitAsteroid.bulletHandle);
        if (bulletLifetime) bulletLifetime->isDead = true;

        // score
        auto& playerControlleds = worldContext.getEntityManager().getPlayerControlled().getAll();
        for (auto& [index, playerControlled] : playerControlleds) {
            switch (asteroid->size) {
            case AsteroidSize::Large:  playerControlled.score += 20;  break;
            case AsteroidSize::Medium: playerControlled.score += 50;  break;
            case AsteroidSize::Small:  playerControlled.score += 100; break;
            default: break;
            }
        }

        Lifetime* asteroidLifetime = worldContext.getEntityManager().getLifetimes().get(bulletHitAsteroid.asteroidHandle);
        if (asteroidLifetime) asteroidLifetime->isDead = true;
    }
}

void AsteroidResponseSystem::spawnAsteroid(WorldContext& worldContext, Vec2 position, Vec2 velocity, AsteroidSize size) {
	auto& entityManager = worldContext.getEntityManager();
	EntityHandle newAsteroid = entityManager.create();

	int spriteSize = static_cast<int>(size) * 8;
	float radius = static_cast<float>(static_cast<int>(size)) * 7.0f;

	entityManager.getAsteroids().add(newAsteroid, { size });
	entityManager.getTransforms().add(newAsteroid, Transform{ position, 0.0f });
	entityManager.getVelocities().add(newAsteroid, Velocity{ velocity, 0.0f });
	entityManager.getSprites().add(newAsteroid, Sprite{ spriteSize, spriteSize, 255, 100, 100 });
	entityManager.getColliders().add(newAsteroid, Collider{ Circle{ radius } });
	entityManager.getNames().add(newAsteroid, Name{ "Asteroid" });
	entityManager.getLifetimes().add(newAsteroid, Lifetime{ 0.0f, false });
}