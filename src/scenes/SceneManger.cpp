#include "pch.h"

#include "SceneManager.hpp"

void SceneManager::loadScene(std::string sceneLocation) {
	sceneDron.load(sceneLocation);
}