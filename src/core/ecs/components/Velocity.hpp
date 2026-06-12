#pragma once

#include "../../math/Vec2.hpp"

struct Velocity {
	Vec2 linearVelocity{ 0.0f, 0.0f };
	float angularVelocity = 0.0f;
};