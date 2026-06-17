#pragma once

enum class AsteroidSize :int {
	Dead = 0,
	Small = 1,
	Medium = 2,
	Large = 3
};

struct Asteroid {
	AsteroidSize size = AsteroidSize::Large;
};