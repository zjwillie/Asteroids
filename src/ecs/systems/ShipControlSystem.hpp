#pragma once

#include <SDL3/SDL.h>

#include "System.hpp"
#include "../../input/InputAction.hpp"

class ShipControlSystem : public System {
public:
	ShipControlSystem() = default;

	void update(WorldContext& worldContext, float deltaTime) override;
	void render(WorldContext& worldContext) override {};

private:
	ShipBindings shipBindings_;
	float turnSpeed_{ 0.0f };
	float thrustForce_{ 0.0f };
	float maxSpeed_{ 10.0f };
	float drag_{ 0.01f };
};