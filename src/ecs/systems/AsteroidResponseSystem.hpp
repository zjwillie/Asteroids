#pragma once

#include "System.hpp"

#include "../../math/Vec2.hpp"
#include "../components/Asteroid.hpp"

class AsteroidResponseSystem : public System {
public:
	void update(WorldContext& worldContext, float deltaTime) override;
	void render(WorldContext& worldContext) override {};
};