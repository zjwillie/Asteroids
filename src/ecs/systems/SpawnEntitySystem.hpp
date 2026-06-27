#pragma once

#include "System.hpp"
#include "../ComponentFactory.hpp"

class SpawnEntitySystem : public System {
public:
	void update(WorldContext& worldContext, float deltaTime) override;
	void render(WorldContext& worldContext) override {};

private:
	ComponentFactory factory_;
};
