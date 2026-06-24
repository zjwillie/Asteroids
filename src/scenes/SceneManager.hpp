#pragma once

#include <string>

#include "DronConfig.hpp"

class SceneManager {
public:
	void loadScene(std::string sceneLocation);
	DronReader sceneDron{};
	std::string currentScene{};
};