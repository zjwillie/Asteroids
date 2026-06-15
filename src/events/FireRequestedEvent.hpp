#pragma once

#include "../math/Vec2.hpp"

struct FireRequestedEvent {
	Vec2 position{ 0,0 };
	float direction{ 0.0f };
	float speedDifferential{ 0.0f };
};