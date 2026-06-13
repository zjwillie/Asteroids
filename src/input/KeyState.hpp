#pragma once

#include <array>
#include <cstdint>

struct KeyState {
	// is the key down
	bool isDown{ false };

	// was the key pressed this frame
	bool wasPressed{ false };

	// was the key released this frame
	bool wasReleased{ false };

	// how many times has the key been pressed (size) and when (index value)
	std::array<float, 16> pressTimestamps{};
	uint32_t timestampHead{ 0 };
	uint32_t totalPressCount{ 0 };

	// when was the last time the key went down
	double lastPressTime{ 0.0 };

	// when was the last time the key went up
	double lastReleaseTime{ 0.0 };
};