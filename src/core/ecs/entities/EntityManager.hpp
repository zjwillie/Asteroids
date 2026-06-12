#pragma once
#include <cstdint>
#include <vector>

#include "EntityHandle.hpp"
#include "../components/ComponentStore.hpp"
#include "../components/Transform.hpp"
#include "../components/Velocity.hpp"
#include "../components/Sprite.hpp"

class EntityManager {
public:
	EntityHandle create();
	void destroy(const EntityHandle& handle);
	bool isAlive(const EntityHandle& handle);

	ComponentStore<Transform>& getTransforms();
	ComponentStore<Velocity>& getVelocities();
	ComponentStore<Sprite>& getSprites();

private:
	std::vector<uint32_t> generations_;
	std::vector<uint32_t> freeList_;

	ComponentStore<Transform> transforms_;
	ComponentStore<Velocity> velocities_;
	ComponentStore<Sprite> sprites_;
};
