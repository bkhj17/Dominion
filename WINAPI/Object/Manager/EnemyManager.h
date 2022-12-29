#pragma once
class EnemyManager : public PoolingManager, public Singleton<EnemyManager>
{
	const float SPAWN_TIME = 1.0f;
private:
	friend class Singleton;
	EnemyManager();
	~EnemyManager();

public:
	virtual void CreateObjects(string key, UINT poolSize) override;

	virtual void Update();
	virtual void Render(HDC hdc);

	Enemy* Collision(const GameObject* object);

	void SetTarget(GameObject* target);
private:
	float playTime = 0.0f;
};

