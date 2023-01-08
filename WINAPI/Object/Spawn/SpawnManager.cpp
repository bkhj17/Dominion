#include "framework.h"
#include "SpawnManager.h"

SpawnManager::SpawnManager()
{
	//spawnObjects.reserve(POOL_SIZE);
	spawnObjects.resize(POOL_SIZE);
	for (auto& object : spawnObjects) {	//참조로 안 하면 주소만 받고 값이 샌다. 중요!!
		object = new SpawnObject();
	}

	cardRect = new Rect(Vector2(), Vector2(50.0f, 50.0f));
}

SpawnManager::~SpawnManager()
{
	for (auto spawnObject : spawnObjects) {
		delete spawnObject;
	}

	delete cardRect;
}

void SpawnManager::Update()
{
	/*
	playTime += DELTA;
	if (playTime >= SPAWN_TIME) {
		for (auto spawnObject : spawnObjects) {
			if (!spawnObject->Active()) {
				spawnObject->Spawn();
				playTime = 0.0f;
				break;
			}
		}
	}
	*/
	if (KEY_DOWN(VK_LBUTTON)) {

	}

	for (auto spawnObject : spawnObjects) {
		spawnObject->Update();
	}

	cardRect->pos = mousePos;
}

void SpawnManager::Render(HDC hdc)
{
	for (auto spawnObject : spawnObjects) {
		spawnObject->Render(hdc);
	}

	cardRect->Render(hdc);

	wstring scoreText = L"Score : " + to_wstring(score);
	TextOut(hdc, (int)CENTER_X, 0, scoreText.c_str(), (int)scoreText.length());
}

void SpawnManager::DestroyObject(Vector2 point)
{
	for (auto spawnObject : spawnObjects) {

		if (spawnObject->IsCollision(cardRect)) {
			spawnObject->Destroy();
			score++;
			return;
		}
	}
}
