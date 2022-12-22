#include "Framework.h"

EnemyManager::EnemyManager()
{
	CreateObjects("Enemy", 50);
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::CreateObjects(string key, UINT poolSize)
{

	vector<GameObject*> enemys(poolSize);

	for (GameObject*& enemy : enemys) {
		enemy = new Enemy;
	}

	totalObjects[key] = enemys;

}

void EnemyManager::Update()
{
	playTime += DELTA;

	if (playTime >= SPAWN_TIME) {
		Enemy* enemy = (Enemy*)Pop("Enemy");
		if (enemy != nullptr) {
			enemy->Spawn();
			playTime = 0.0f;
		}
	}

	__super::Update();
}

void EnemyManager::Render(HDC hdc)
{
	vector<GameObject*> enemies = totalObjects["Enemy"];

	for (GameObject* enemy : enemies) {
		enemy->Render(hdc);
	}
}

Enemy* EnemyManager::Collision(const GameObject* object)
{
	vector<GameObject*> enemies = totalObjects["Enemy"];

	for (GameObject* enemy : enemies) {
		if (enemy->IsCollision(object))
			return (Enemy*)enemy;
	}

	return nullptr;
}

void EnemyManager::SetTarget(GameObject* target)
{

	vector<GameObject*> enemies = totalObjects["Enemy"];

	for (GameObject* enemy : enemies) {
		((Enemy*)enemy)->SetTarget(enemy);
	}
}
