#pragma once

class SpawnManager : public Singleton<SpawnManager>
{
private:
	const UINT POOL_SIZE = 50;
	const float SPAWN_TIME = 1.0f;

	friend class Singleton;

	SpawnManager();
	~SpawnManager();
public:

	void Update();
	void Render(HDC hdc);

	void DestroyObject(Vector2 point);

public:
	vector<SpawnObject*> spawnObjects;

	float playTime = 0.0f;

	int score = 0;


	Rect* rect;
};

