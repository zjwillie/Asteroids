#include "pch.h"

#include "SceneManager.hpp"

void SceneManager::loadScene(std::string sceneLocation) {
	DronReader sceneDron{ DronConfig::load("../assets/scenes/game.scene") };

}