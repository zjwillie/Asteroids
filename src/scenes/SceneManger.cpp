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
    levelData.asteroids = dronLevel["Asteroid"];

    loadAsteroidArchetypes();
}

// Load the three flat asteroid archetypes once. Cached as DronReaders; callers
// take copies via getAsteroidArchetype(). The only place these paths live.
void SceneManager::loadAsteroidArchetypes() {
    asteroidArchetypes_["LARGE"].load("assets/entities/asteroid_large.ent");
    asteroidArchetypes_["MEDIUM"].load("assets/entities/asteroid_medium.ent");
    asteroidArchetypes_["SMALL"].load("assets/entities/asteroid_small.ent");
}

DronDocument SceneManager::getAsteroidArchetype(const std::string& size) const {
    auto it = asteroidArchetypes_.find(size);
    if (it == asteroidArchetypes_.end()) {
        LOG_WARN("SceneManager", "No asteroid archetype for size '{}'", size);
        return DronDocument{};
    }
    return it->second.snapshot();
}

// Recursively merge `overrides` into `target`: scalars/lists replace, nested
// maps merge key-by-key (so an override touching [Transform].x leaves .y intact).
static void mergeInto(DronMap& target, const DronMap& overrides) {
    for (const auto& [key, node] : overrides) {
        auto it = target.find(key);
        if (node.isMap() && it != target.end() && it->second.isMap()) {
            mergeInto(it->second.as<DronMap>(), node.as<DronMap>());
        }
        else {
            target[key] = node;
        }
    }
}

void SceneManager::applyOverrides(const std::string& name, DronDocument& doc) const {
    DronValue overrides = dronLevel["Overrides"][name];
    if (!overrides.isMap()) return;   // nothing to override for this entity
    mergeInto(doc, overrides.as<DronMap>());
}

void SceneManager::debugPrint() {
    LOG_INFO("SceneManager", "currentScene: {}", currentScene);
    LOG_INFO("SceneManager", "currentLevel: {}", currentLevel);
    LOG_INFO("SceneManager", "--- scene ---\n{}", dronScene.dump());
    LOG_INFO("SceneManager", "--- level ---\n{}", dronLevel.dump());
}