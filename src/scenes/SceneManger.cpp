#include "pch.h"

#include "SceneManager.hpp"
#include "../utilities/logger/Logger.hpp"

void SceneManager::loadScene(std::string sceneLocation) {
	sceneDron.load(sceneLocation);

	currentScene = sceneDron["Scene"]["name"];
	LOG_INFO("Game", "Loading the scene : {}", currentScene);
}

//	std::string sceneName = sceneManager_.sceneDron["Scene"]["name"];