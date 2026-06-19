#pragma once

#include "System.hpp"

#include "../../math/Vec2.hpp"

class ShipResponseSystem : public System {
public:
	void update(WorldContext& worldContext, float deltaTime) override;
	void render(WorldContext& worldContext) override {};
};