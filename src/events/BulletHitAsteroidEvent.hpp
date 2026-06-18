#pragma once

#include "../ecs/entities/EntityHandle.hpp"

struct BulletHitAsteroidEvent {
	EntityHandle bulletHandle;
	EntityHandle asteoridHandle;
};