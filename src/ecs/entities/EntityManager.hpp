#pragma once
#include <cstdint>
#include <vector>

#include "EntityHandle.hpp"
#include "../components/ComponentStore.hpp"
#include "../components/Transform.hpp"
#include "../components/Velocity.hpp"
#include "../components/Sprite.hpp"
#include "../components/PlayerControlled.hpp"
#include "../components/Lifetime.hpp"
#include "../components/Bullet.hpp"
#include "../components/Name.hpp"

class EntityManager {
public:
	EntityHandle create();
	void destroy(const EntityHandle& handle);
	bool isAlive(const EntityHandle& handle);
	EntityHandle getHandle(uint32_t index) const;

	ComponentStore<Transform>& getTransforms();
	ComponentStore<Velocity>& getVelocities();
	ComponentStore<Sprite>& getSprites();
	ComponentStore<PlayerControlled>& getPlayerControlled();
	ComponentStore<Lifetime>& getLifetimes();
	ComponentStore<Bullet>& getBullets();
	ComponentStore<Name>& getNames();

private:
	std::vector<uint32_t> generations_;
	std::vector<uint32_t> freeList_;

	ComponentStore<Transform> transforms_;
	ComponentStore<Velocity> velocities_;
	ComponentStore<Sprite> sprites_;
	ComponentStore<PlayerControlled> playerControlled_;
	ComponentStore<Lifetime> lifetimes_;
	ComponentStore<Bullet> bullets_;
	ComponentStore<Name> names_;
};
