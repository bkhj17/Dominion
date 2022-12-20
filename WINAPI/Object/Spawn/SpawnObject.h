#pragma once
class SpawnObject : public Rect
{
private:
	//UINT는 windows에서 이미 정의되어 있다
	const float MIN_SIZE = 50.0f;
	const float MAX_SIZE = 200.0f;
	
	const float MIN_LIFE_TIME = 3.0f;
	const float MAX_LIFE_TIME = 10.0f;
public:
	SpawnObject();
	~SpawnObject();

	void Init();
	void Update();
	void Render(HDC hdc);

	void Spawn();
	void Destroy();
private:
	float lifeTime = 0.0f;	//생존시간
	float playTime = 0.0f;	//현재시간

	HBRUSH hBrush = nullptr;
};

