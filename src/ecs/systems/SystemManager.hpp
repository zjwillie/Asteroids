#pragma once

#include <memory>
#include <vector>

#include "System.hpp"

class WorldContext;

class SystemManager {
public:
	void registerSystem(std::unique_ptr<System> system);
	void update(WorldContext& worldContext, float deltaTime);
	void render(WorldContext& worldContext);

private:
	std::vector<std::unique_ptr<System>> systems_;
};