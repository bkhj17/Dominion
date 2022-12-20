#include "framework.h"
#include "SpawnScene.h"


SpawnScene::SpawnScene()
{
	SpawnManager::Get();

}

SpawnScene::~SpawnScene()
{
	SpawnManager::Delete();

}

void SpawnScene::Init()
{
}

void SpawnScene::Update()
{
	SpawnManager::Get()->DestroyObject(mousePos);

	SpawnManager::Get()->Update();
}

void SpawnScene::Render(HDC hdc)
{
	SpawnManager::Get()->Render(hdc);

}
