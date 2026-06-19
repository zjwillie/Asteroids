#pragma once


struct PlayerControlled {
	bool currently_controllable = true;
	int lives = 3;
	int score = 0;

	bool isInvulnerable = false;
	float invulnerableTimer = 0.0f;
};