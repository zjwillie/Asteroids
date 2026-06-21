#include "pch.h"

#include <vector>
#include <cmath>

#include "ShipResponseSystem.hpp"

#include "../../core/WorldContext.hpp"
#include "../entities/EntityManager.hpp"
#include "../../events/EventManager.hpp"
#include "../../math/Vec2.hpp"
#include "../../utilities/logger/Logger.hpp"

void ShipResponseSystem::update(WorldContext& worldContext, float deltaTime) {
    const auto& shipHitAsteroidEvents = worldContext.getEventManager().getShipHitAsteroidEvents();
    auto& playerControlleds = worldContext.getEntityManager().getPlayerControlled();

    // tick invulnerability every frame
    for (auto& [index, playerControlled] : playerControlleds.getAll()) {
        if (playerControlled.isInvulnerable) {
            playerControlled.invulnerableTimer -= deltaTime;
            if (playerControlled.invulnerableTimer <= 0.0f) {
                playerControlled.isInvulnerable = false;
                playerControlled.invulnerableTimer = 0.0f;
            }
        }
    }

    if (!shipHitAsteroidEvents.empty()) {
        const ShipHitAsteroidEvent& event = shipHitAsteroidEvents[0];

        if (!worldContext.getEntityManager().isAlive(event.shipHandle)) return;
        if (!worldContext.getEntityManager().isAlive(event.asteroidHandle)) return;

        PlayerControlled* playerControlled = playerControlleds.get(event.shipHandle);
        if (!playerControlled || playerControlled->isInvulnerable) return;

        // ship hit — lose a life, respawn at center with invulnerability
        playerControlled->lives -= 1;
        playerControlled->isInvulnerable = true;
        playerControlled->invulnerableTimer = 3.0f;

        Transform* transform = worldContext.getEntityManager().getTransforms().get(event.shipHandle);
        Velocity* velocity = worldContext.getEntityManager().getVelocities().get(event.shipHandle);
        if (transform) { transform->position = Vec2{ 160.0f, 90.0f }; transform->rotation = 0.0f; }
        if (velocity) { velocity->linearVelocity = Vec2{ 0.0f, 0.0f }; velocity->angularVelocity = 0.0f; }

        LOG_DEBUG("Ship", "Ship hit! Lives remaining: {}", playerControlled->lives);
    }
}