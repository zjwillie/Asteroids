#pragma once

#include "System.hpp"

class WorldContext;

class WeaponSystem : public System {
public:
	void update(WorldContext& worldContext, float deltaTime) override;
	void render(WorldContext& worldContext) override {};
private:
	float bulletSpeed_{ 300.0f };
};