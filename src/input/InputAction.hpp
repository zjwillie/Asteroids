// TODO Phase 5: add string conversion LUT for loading bindings from .bindings files
// See MECS3 InputActionEnum.hpp for reference implementation

#pragma once

#include <cstdint>
#include <SDL3/SDL.h>

enum class InputAction : uint8_t {
    Unknown = 0,
    Thrust,
    RotateLeft,
    RotateRight,
    Fire,
    Pause
};

// Temporary hard code binding
struct ShipBindings {
    SDL_Scancode thrust = SDL_SCANCODE_UP;
    SDL_Scancode rotateLeft = SDL_SCANCODE_LEFT;
    SDL_Scancode rotateRight = SDL_SCANCODE_RIGHT;
    SDL_Scancode fire = SDL_SCANCODE_SPACE;
};