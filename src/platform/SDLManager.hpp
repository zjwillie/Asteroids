#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <vector>

class SDLManager {
public:
    bool initialize(const std::string& title, int width, int height);
    void shutdown();

    bool pollEvents(std::vector<SDL_Event>& outEvents);
    void clear(uint8_t red, uint8_t green, uint8_t blue);
    void present();

    float getDeltaTime();
    SDL_Renderer* getRenderer() const { return renderer_; }

private:
    static constexpr float kMaxDeltaTime = 0.05f;

    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;

    // delta time tracking
    uint64_t lastCounter_ = 0;
};