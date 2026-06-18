#pragma once

#include "../ecs/entities/EntityHandle.hpp"

struct ShipHitAsteroidEvent {
	EntityHandle shipHandle;
	EntityHandle asteroidHandle;
};