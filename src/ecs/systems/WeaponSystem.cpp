#include "pch.h"

#include <vector>

#include "WeaponSystem.hpp"

#include "../../core/WorldContext.hpp"
#include "../entities/EntityManager.hpp"
#include "../../events/EventManager.hpp"
#include "../../events/FireRequestedEvent.hpp"
#include "Logger.hpp"

void WeaponSystem::update(WorldContext& worldContext, float) {
    const auto& fireRequests = worldContext.getEventManager().getFireRequests();

    for (const FireRequestedEvent& fireRequest : fireRequests) {
        Vec2 facing = { std::cos(fireRequest.direction), std::sin(fireRequest.direction) };
        Vec2 bulletVelocity = facing * (bulletSpeed_ + fireRequest.speedDifferential);

        LOG_DEBUG("Weapon", "Bullet spawned at {:.1f}, {:.1f}", fireRequest.position.x, fireRequest.position.y);
        EntityHandle bullet = worldContext.getEntityManager().create();
        worldContext.getEntityManager().getNames().add(bullet, Name{ "Bullet" });
        worldContext.getEntityManager().getTransforms().add(bullet, Transform{ fireRequest.position, fireRequest.direction });
        worldContext.getEntityManager().getVelocities().add(bullet, Velocity{ bulletVelocity, 0.0f });
        worldContext.getEntityManager().getSprites().add(bullet, Sprite{ 3, 3, 255, 255, 0 });
        worldContext.getEntityManager().getLifetimes().add(bullet, Lifetime{ 0.5f, false });
        worldContext.getEntityManager().getBullets().add(bullet, Bullet{});
        worldContext.getEntityManager().getColliders().add(bullet, Collider{ Circle{ 2.0f } });
    }
}