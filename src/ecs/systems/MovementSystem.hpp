#pragma once

#include "System.hpp"

class MovementSystem : public System {
public:
	void update(WorldContext& worldContext, float deltaTime) override;
	void render(WorldContext& worldContext) override {};
};