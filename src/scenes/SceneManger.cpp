#include "pch.h"

#include "SceneManager.hpp"

#include "../events/SpawnEntityEvent.hpp"
#include "Logger.hpp"

void SceneManager::loadScene(std::string sceneLocation) {
    dronScene.load(sceneLocation);

    currentScene = dronScene["Scene"]["name"];
    currentLevel = dronScene["Level"]["file"];

    LOG_FATAL("Scenemanager", "SCENE: {}", currentScene);

    dronLevel.load(currentLevel);

    sceneData.entities = dronScene["Entities"];
    sceneData.transitions = dronScene["Transitions"];
    sceneData.systems = dronScene["Systems"];

    levelData.entities = dronLevel["Entities"];
    levelData.ship = dronLevel["Ship"];
    levelData.asteroids = dronLevel["Asteroids"];
}

void SceneManager::debugPrint() {
    LOG_INFO("SceneManager", "currentScene: {}", currentScene);
    LOG_INFO("SceneManager", "currentLevel: {}", currentLevel);
    LOG_INFO("SceneManager", "--- scene ---\n{}", dronScene.dump());
    LOG_INFO("SceneManager", "--- level ---\n{}", dronLevel.dump());
}