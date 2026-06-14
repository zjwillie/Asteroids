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
// Can assign multiple keys with differnt words thrustAlt if we wanted two
struct ShipBindings {
    SDL_Scancode thrust = SDL_SCANCODE_W;
    SDL_Scancode rotateLeft = SDL_SCANCODE_A;
    SDL_Scancode rotateRight = SDL_SCANCODE_D;
    SDL_Scancode fire = SDL_SCANCODE_SPACE;
};