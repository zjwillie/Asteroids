#pragma once

// =============================================================================
// Precompiled header
//
// Holds heavy, stable, widely-used third-party and standard-library headers so
// they are parsed once instead of in every translation unit. Only headers used
// across multiple .cpp/.hpp files belong here. Project/game headers are
// intentionally excluded so changing them does not force a full rebuild.
// =============================================================================

// Third-party
#include <SDL3/SDL.h>

// C++ standard library
#include <cmath>
#include <cstdint>
#include <format>
#include <fstream>
#include <memory>
#include <random>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
