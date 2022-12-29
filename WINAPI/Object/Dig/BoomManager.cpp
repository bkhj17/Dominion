#include "framework.h"
#include "BoomManager.h"

BombManager::BombManager()
{
	hBrush = CreateSolidBrush(MAGENTA);
	hPen = CreatePen(PS_SOLID, 10, MAGENTA);
	CreateObjects("Bomb", POOL_SIZE);

}

BombManager::~BombManager()
{
}

void BombManager::Plant(const Vector2 pos)
{
	Bomb* bomb = (Bomb*)Pop("Bomb");
	if (bomb == nullptr)
		return;
	bomb->Plant(pos);
}

void BombManager::CreateObjects(string key, UINT poolSize)
{
	vector<GameObject*> bombs(poolSize);

	for (auto& bomb : bombs) {
		bomb = new Bomb();
		((Bomb*)bomb)->SetBrush(hBrush, hPen);
	}

	totalObjects[key] = bombs;
}

void BombManager::SetLand(Texture* texture)
{
	for (auto& object : totalObjects["Bomb"]) {
		Bomb* bomb = (Bomb*)object;
		bomb->SetLand(texture);
	}
}

void BombManager::Clear()
{
	vector<GameObject*> bombs = totalObjects["Bomb"];
	for (GameObject* bomb : bombs) {
		delete bomb;
	}
	bombs.clear();
}
