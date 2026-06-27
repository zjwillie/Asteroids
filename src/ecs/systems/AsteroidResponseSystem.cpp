#include "pch.h"

#include <cmath>

#include "AsteroidResponseSystem.hpp"

#include "../../core/WorldContext.hpp"
#include "../entities/EntityManager.hpp"
#include "../../events/EventManager.hpp"
#include "../../events/SpawnEntityEvent.hpp"
#include "../../scenes/SceneManager.hpp"
#include "Logger.hpp"
#include "../../utilities/random/Random.hpp"

namespace {
    // Hardcoded split rules for now: large -> 2 medium, medium -> 2 small,
    // small -> none. Returns the archetype name for the next size down, or
    // nullptr if this size does not split.
    const char* nextSizeDown(AsteroidSize size) {
        switch (size) {
        case AsteroidSize::Large:  return "MEDIUM";
        case AsteroidSize::Medium: return "SMALL";
        default:                   return nullptr;   // Small / Dead: no split
        }
    }
    constexpr int SPLIT_COUNT = 2;
}

void AsteroidResponseSystem::update(WorldContext& worldContext, float deltaTime) {
    EntityManager& entityManager = worldContext.getEntityManager();
    EventManager& eventManager = worldContext.getEventManager();
    SceneManager& sceneManager = worldContext.getSceneManager();

    const auto& bulletHitAsteroidEvents = eventManager.getBulletHitAsteroidEvents();

    for (const BulletHitAsteroidEvent bulletHitAsteroid : bulletHitAsteroidEvents) {
        if (!entityManager.isAlive(bulletHitAsteroid.bulletHandle)) continue;
        if (!entityManager.isAlive(bulletHitAsteroid.asteroidHandle)) continue;

        Name* asteroidName = entityManager.getNames().get(bulletHitAsteroid.asteroidHandle);
        LOG_DEBUG("AsteroidResponseSystem", "Destroying {} ({}) '{}'",
            bulletHitAsteroid.asteroidHandle.index,
            bulletHitAsteroid.asteroidHandle.generation,
            asteroidName ? asteroidName->name : "unnamed"
        );

        Asteroid* asteroid = entityManager.getAsteroids().get(bulletHitAsteroid.asteroidHandle);
        Transform* asteroidTransform = entityManager.getTransforms().get(bulletHitAsteroid.asteroidHandle);
        Velocity* asteroidVelocity = entityManager.getVelocities().get(bulletHitAsteroid.asteroidHandle);

        // Split: emit SpawnEntityEvents for the next size down. Creation is fully
        // deferred to the factory pipeline — no entityManager.create() here.
        if (asteroid && asteroidTransform && asteroidVelocity) {
            const char* smallerArchetype = nextSizeDown(asteroid->size);
            if (smallerArchetype) {
                float baseSpeed = asteroidVelocity->linearVelocity.length() * 1.3f;
                Vec2 direction = asteroidVelocity->linearVelocity.normalized();
                float spreadAngle = (2.0f * 3.14159f) / SPLIT_COUNT;

                for (int i = 0; i < SPLIT_COUNT; ++i) {
                    float angle = spreadAngle * i + Random::floatRange(-5.5f, 5.5f);
                    float speed = baseSpeed * Random::floatRange(0.8f, 2.3f);

                    Vec2 spawnVelocity = Vec2{
                        direction.x * std::cos(angle) - direction.y * std::sin(angle),
                        direction.x * std::sin(angle) + direction.y * std::cos(angle)
                    } * speed;

                    // Grab a fresh archetype copy, stamp placement, emit. The
                    // SpawnEntitySystem turns the document into an entity.
                    DronWriter blueprint{ sceneManager.getAsteroidArchetype(smallerArchetype) };
                    blueprint["Transform"]["x"] = static_cast<double>(asteroidTransform->position.x);
                    blueprint["Transform"]["y"] = static_cast<double>(asteroidTransform->position.y);
                    blueprint["Velocity"]["dx"] = static_cast<double>(spawnVelocity.x);
                    blueprint["Velocity"]["dy"] = static_cast<double>(spawnVelocity.y);

                    SpawnEntityEvent spawnEvent{};
                    spawnEvent.entity = blueprint.document();
                    eventManager.emit(spawnEvent);
                }
            }
        }

        Lifetime* bulletLifetime = entityManager.getLifetimes().get(bulletHitAsteroid.bulletHandle);
        if (bulletLifetime) bulletLifetime->isDead = true;

        // score
        if (asteroid) {
            auto& playerControlleds = entityManager.getPlayerControlled().getAll();
            for (auto& [index, playerControlled] : playerControlleds) {
                switch (asteroid->size) {
                case AsteroidSize::Large:  playerControlled.score += 20;  break;
                case AsteroidSize::Medium: playerControlled.score += 50;  break;
                case AsteroidSize::Small:  playerControlled.score += 100; break;
                default: break;
                }
            }
        }

        Lifetime* asteroidLifetime = entityManager.getLifetimes().get(bulletHitAsteroid.asteroidHandle);
        if (asteroidLifetime) asteroidLifetime->isDead = true;
    }
}
