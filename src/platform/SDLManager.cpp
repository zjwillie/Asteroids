#include <SDL3/SDL.h>

#include "SDLManager.hpp"
#include "../utilities/logger/Logger.hpp"

float SDLManager::getDeltaTime() {
    uint64_t currentCounter = SDL_GetPerformanceCounter();
    uint64_t elapsed = currentCounter - lastCounter_;
    lastCounter_ = currentCounter;

    float deltaTime = static_cast<float>(elapsed) / static_cast<float>(SDL_GetPerformanceFrequency());

    if (deltaTime > kMaxDeltaTime) deltaTime = kMaxDeltaTime;

    return deltaTime;
}

bool SDLManager::initialize(const std::string& title, int width, int height) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        LOG_FATAL("SDL", "SDL_Init failed: {}", SDL_GetError());
        return false;
    }

    window_ = SDL_CreateWindow(title.c_str(), width, height, 0);
    if (!window_) {
        LOG_FATAL("SDL", "SDL_CreateWindow failed: {}", SDL_GetError());
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, nullptr);
    if (!renderer_) {
        LOG_FATAL("SDL", "SDL_CreateRenderer failed: {}", SDL_GetError());
        return false;
    }

    // 320x180 design resolution - integer scale only to preserve pixel art crispness
    SDL_SetRenderLogicalPresentation(renderer_, 320, 180,
        SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);

    lastCounter_ = SDL_GetPerformanceCounter();
    return true;
}

bool SDLManager::pollEvents(std::vector<SDL_Event>& outEvents) {
    outEvents.clear();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) return false;
        outEvents.push_back(event);
    }
    return true;
}

void SDLManager::clear(uint8_t red, uint8_t green, uint8_t blue) {
    SDL_SetRenderDrawColor(renderer_, red, green, blue, 255);
    SDL_RenderClear(renderer_);
}

void SDLManager::shutdown() {
    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;
    SDL_DestroyWindow(window_);
    window_ = nullptr;
    SDL_Quit();
}

void SDLManager::present() {
    SDL_RenderPresent(renderer_);
}