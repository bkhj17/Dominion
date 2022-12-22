#include "Framework.h"
#include "ShootingScene.h"

ShootingScene::ShootingScene()
{
	plane = new Plane;
	BulletManager::Get();
	EnemyManager::Get()->SetTarget(plane);
}

ShootingScene::~ShootingScene()
{
	delete plane;
	BulletManager::Delete();
	EnemyManager::Delete();
}

void ShootingScene::Init()
{
}

void ShootingScene::Update()
{
	BulletManager::Get()->CollisionBullet("PlayerBullet", "EnemyBullet");

	plane->Update();

	BulletManager::Get()->Update();
	EnemyManager::Get()->Update();
}

void ShootingScene::Render(HDC hdc)
{
	plane->Render(hdc);
	BulletManager::Get()->Render(hdc);
	EnemyManager::Get()->Render(hdc);
}
