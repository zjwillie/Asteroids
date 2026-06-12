#pragma once

class WorldContext;

class System {
public:
	virtual void update(WorldContext& world, float deltaTime) = 0;
	virtual void render(WorldContext& world) = 0;
	virtual ~System() = default;
};