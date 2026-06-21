#include "pch.h"

#include "Random.hpp"

#include <random>

std::mt19937 Random::rng_;

void Random::initialize() {
	rng_.seed(std::random_device{}());
}

void Random::seed(uint32_t fixedSeed) {
	rng_.seed(fixedSeed);
}

float Random::floatRange(float min, float max) {
	std::uniform_real_distribution<float> dist(min, max);
	return dist(rng_);
}

int Random::intRange(int min, int max) {
	std::uniform_int_distribution<int> dist(min, max);
	return dist(rng_);
}

int Random::intRange(int min, int max, int offset) {
	std::uniform_int_distribution<int> dist(min, max);
	return dist(rng_) + offset;
}

bool Random::chance(float probability) {
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);
	float chance = dist(rng_);
	// remember we should offest probability a little or ensure that we track fails, because
	// reality is less fun and the player needs to feel lucky sometimes,
	// yes I know, asteroids doesn't need it but we will need it went we build bigger dreams
	return chance <= probability;
}