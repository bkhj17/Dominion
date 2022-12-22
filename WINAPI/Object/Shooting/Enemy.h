#pragma once

class Enemy : public Rect {
private:
	//UINT는 windows에서 이미 정의되어 있다
	const float MIN_SIZE = 50.0f;
	const float MAX_SIZE = 100.0f;

	const float MIN_SPEED = 100.0f;
	const float MAX_SPEED = 400.0f;

	const float FIRE_TIME = 2.0f;
public:
	Enemy();
	~Enemy();

	void Init();
	void Update();
	void Render(HDC hdc);

	void Spawn();
	void Destroy();

	void Damage();

	void Fire();

	void SetTarget(GameObject* target) { this->target = target; }
private:
	float speed = 0.0f;
	float fireTime = 0.0f;

	HBRUSH hBrush = nullptr;
	HPEN hPen = nullptr;

	GameObject* target = nullptr;
};