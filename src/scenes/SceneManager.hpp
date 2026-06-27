#pragma once

#include <string>
#include <unordered_map>

#include "DronConfig.hpp"

struct SceneData {
    DronValue entities;
    DronValue transitions;
    DronValue systems;
};

struct LevelData {
    DronValue entities;
    DronValue ship;
    DronValue asteroids;
};

class SceneManager {
public:
    void loadScene(std::string sceneLocation);
    std::string currentScene{};
    std::string currentLevel{};

    DronReader dronScene{};
    DronReader dronLevel{};

    SceneData sceneData{};
    LevelData levelData{};

    // Returns a fresh copy of an asteroid archetype document ("LARGE"/"MEDIUM"/
    // "SMALL"), loaded once and cached. Empty document if the size is unknown.
    // All DrON file IO stays in here; callers just stamp + emit the copy.
    DronDocument getAsteroidArchetype(const std::string& size) const;

    // The standard spawn hook: deep-merge the current level's [Overrides] for the
    // named entity over its loaded document (override values win). No-op if the
    // level has no overrides for `name`. Every named-entity spawn runs through it.
    void applyOverrides(const std::string& name, DronDocument& doc) const;

    void debugPrint();

private:
    void loadAsteroidArchetypes();
    std::unordered_map<std::string, DronReader> asteroidArchetypes_;
};