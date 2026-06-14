#pragma once

#include "System.hpp"

class WorldContext;

class WarpSystem : public System {
public:
	void update(WorldContext& worldContext, float deltaTime) override;
	void render(WorldContext& worldContext) override {};
private:
	float playAreaWidth_ = 320.0f;
	float playAreaHeight_ = 180.0f;
	float margin_ = 16.0f;
};