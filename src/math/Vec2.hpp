#pragma once

#include <cmath>

struct Vec2 {
	float x, y;

	Vec2 operator+(const Vec2& other) const {
		return Vec2{ x + other.x, y + other.y };
	}

	Vec2 operator-(const Vec2& other) const {
		return Vec2{ x - other.x, y - other.y };
	}

	Vec2 operator*(const float other) const {
		return Vec2{ x * other, y * other };
	}

	Vec2& operator+=(const Vec2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2& operator-=(const Vec2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	float length() const {
		return sqrt(x * x + y * y);
	}

	Vec2 normalized() const {
		float vectorLength = length();
		if (vectorLength == 0) return Vec2{ 0,0 };
		return Vec2{ x / vectorLength, y / vectorLength };
	}
};