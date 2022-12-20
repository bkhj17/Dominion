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
