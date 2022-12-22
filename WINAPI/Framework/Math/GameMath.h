#pragma once

namespace GameMath {
	enum class Direction {
		UP, DOWN, LEFT, RIGHT, NONE
	};

	int Random(const int& min, const int& max);
	float Random(const float& min, const float& max);
}
