#include "Framework.h"
#include "GameMath.h"

int GameMath::Random(const int& min, const int& max) {
	 return rand() % (max - min) + min;
}

float GameMath::Random(const float& min, const float& max)
{
	float normal = rand() / (float)RAND_MAX;	//랜덤을 0.0~1.0까지의 실수로 변환
	return (max - min) * normal + min;			
}

float GameMath::Clamp(const float& min, const float& max, float value)
{
	if (value < min)
		value = min;

	if (value > max)
		value = max;

	return value;
}

float GameMath::Lerp(const float& start, const float& end, float t)
{
	t = Clamp(0.0f, 1.0f, t);

	return start + t * (end - start);
}

Vector2 GameMath::Lerp(const Vector2& start, const Vector2& end, float t)
{
	t = GameMath::Clamp(0.0f, 1.0f, t);
	return start + (end - start) * t;
}

Vector2 GameMath::SLerp(const Vector2& start, const Vector2& end, float t)
{
	t = GameMath::Clamp(0.0f, 1.0f, t);
	return start + (end - start) * t*t;
}
