#pragma once

#include "System.hpp"
#include "../../input/InputAction.hpp"

class ShipControlSystem : public System {
public:
	ShipControlSystem() = default;

	void update(WorldContext& worldContext, float deltaTime) override;
	void render(WorldContext& worldContext) override {};

private:
	ShipBindings shipBindings_;
	float turnSpeed_{ 3.0f };
	float thrustForce_{ 150.0f };
	float maxSpeed_{ 200.0f };
	float drag_{ 4.0f };
};