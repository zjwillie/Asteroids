#pragma once

#include "System.hpp"

class RenderSystem : public System {
public:
	void update(WorldContext& worldContext, float deltaTime) override {};
	void render(WorldContext& worldContext) override {};
};