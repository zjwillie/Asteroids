#pragma once

#include <random>

class Random {
public:
	static void initialize();

	static void seed(uint32_t fixedSeed);

	static float floatRange(float min, float max);
	static int intRange(int min, int max);
	static int intRange(int min, int max, int offset);
	static bool chance(float probability);

private:
	static std::mt19937 rng_;
};