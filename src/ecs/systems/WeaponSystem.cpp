#include <vector>

#include "WeaponSystem.hpp"

#include "../../core/WorldContext.hpp"
#include "../../events/FireRequestedEvent.hpp"

void WeaponSystem::update(WorldContext& worldContext, float) {
    const auto& fireRequests = worldContext.getEventManager().getFireRequests();

    for (const FireRequestedEvent& fireRequest : fireRequests) {
        Vec2 facing = { std::cos(fireRequest.direction), std::sin(fireRequest.direction) };
        Vec2 bulletVelocity = facing * (bulletSpeed_ + fireRequest.speedDifferential);

        SDL_Log("[%.3f] Bullet spawned", SDL_GetTicks() / 1000.0f);
        SDL_Log("[%.3f] Bullet spawned at %.1f, %.1f", SDL_GetTicks() / 1000.0f, fireRequest.position.x, fireRequest.position.y);
        EntityHandle bullet = worldContext.getEntityManager().create();
        worldContext.getEntityManager().getTransforms().add(bullet, Transform{ fireRequest.position, fireRequest.direction });
        worldContext.getEntityManager().getVelocities().add(bullet, Velocity{ bulletVelocity, 0.0f });
        worldContext.getEntityManager().getSprites().add(bullet, Sprite{ 3, 3, 255, 255, 0 });
        worldContext.getEntityManager().getLifetimes().add(bullet, Lifetime{ 0.5f, false });
        worldContext.getEntityManager().getBullets().add(bullet, Bullet{});
    }
}