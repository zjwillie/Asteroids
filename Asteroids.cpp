#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <vector>

class SDLManager {
public:
    bool initialize(const std::string& title, int width, int height);
    void shutdown();

    bool pollEvents(std::vector<SDL_Event>& outEvents);
    void clear(uint8_t r, uint8_t g, uint8_t b);
    void present();

    float getDeltaTime() const;
    SDL_Renderer* getRenderer() const { return renderer_; }

private:
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;

    // delta time tracking
    uint64_t lastCounter_ = 0;
    float deltaTime_ = 0.0f;
};
bool SDLManager::initialize(const std::string& title, int width, int height) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    window_ = SDL_CreateWindow(title.c_str(), width, height, 0);
    if (!window_) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, nullptr);
    if (!renderer_) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        return false;
    }

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
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void SDLManager::present() {
    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;
    SDL_DestroyWindow(window_);
    window_ = nullptr;
    SDL_Quit();
}

//*******************************************
//*******************************************
//********         MAIN         *************
//*******************************************
//*******************************************

int main(int, char**) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("MECS4", 1280, 720, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN && SDLK_ESCAPE) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}