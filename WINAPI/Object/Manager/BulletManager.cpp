#include "framework.h"
#include "BulletManager.h"

BulletManager::BulletManager()
{
	CreateObjects("PlayerBullet", 50);
	CreateObjects("EnemyBullet", 50);
}

BulletManager::~BulletManager()
{
}

void BulletManager::CreateObjects(string key, UINT poolSize)
{

	vector<GameObject*> bullets(poolSize);

	for (GameObject*& bullet : bullets) {
		bullet = new Bullet;
		bullet->tag = key;
	}

	totalObjects[key] = bullets;
}

void BulletManager::Fire(const Vector2& pos)
{
	Bullet* bullet = dynamic_cast<Bullet*>(Pop("PlayerBullet"));
	if (bullet != nullptr)
		bullet->Fire(pos);
}

void BulletManager::Fire(const Vector2& pos, const Vector2& direction)
{
	Bullet* bullet = dynamic_cast<Bullet*>(Pop("EnemyBullet"));
	if (bullet != nullptr)
		bullet->Fire(pos, direction);
}

bool BulletManager::Collision(const GameObject* object)
{
	const Enemy* enemy = dynamic_cast<const Enemy*>(object);
	if (enemy && enemy != nullptr) {
		vector<GameObject*> bullets = totalObjects["PlayerBullet"];
		for (auto bullet : bullets) {
			if (bullet->isActive && bullet->IsCollision(object)) {
				return true;
			}
		}
	}

	const Plane* plane = dynamic_cast<const Plane*>(object);
	if (plane && plane != nullptr) {
		vector<GameObject*> bullets = totalObjects["EnemyBullet"];
		for (auto bullet : bullets) {
			if (bullet->isActive && bullet->IsCollision(object)) {
				return true;
			}
		}
	}
	return false;
}

void BulletManager::CollisionBullet(string key1, string key2)
{
	vector<GameObject*>& bullets1 = totalObjects[key1];
	vector<GameObject*>& bullets2 = totalObjects[key2];
	
	for (auto& bullet : bullets1) {
		if (!bullet->isActive)
			continue;

		for (auto& bullet2 : bullets2) {
			if (!bullet2->isActive)
				continue;

			if (bullet->IsCollision(bullet2)) {
				bullet->isActive = false;
				bullet2->isActive = false;
				break;
			}
		}
	}
}

GameObject* BulletManager::CollisionLand(Texture* texture)
{
	vector<GameObject*> bullets = totalObjects["EnemyBullet"];
	for (auto bullet : bullets) {
		if (!bullet->isActive)
			continue;

		float height = texture->GetPixelHeight(bullet->pos);
		if (bullet->pos.y > height) {
			return bullet;
		}
	}

	return nullptr;
}