#pragma once

#include <string>

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

    void debugPrint();
};