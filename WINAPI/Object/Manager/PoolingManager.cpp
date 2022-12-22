#include "framework.h"
#include "PoolingManager.h"

PoolingManager::~PoolingManager()
{
	for (pair<string, Objects> objects : totalObjects) {
		for (GameObject* object : objects.second)
			delete object;
	}

}

void PoolingManager::Update()
{
	for (pair<string, Objects> objects : totalObjects) {
		for (GameObject* object : objects.second)
			object->Update();
	}
}

void PoolingManager::Render(HDC hdc)
{
	for (pair<string, Objects> objects : totalObjects) {
		for (GameObject* object : objects.second)
			object->Render(hdc);
	}
}

GameObject* PoolingManager::Pop(string key)
{
	if (totalObjects.find(key) == totalObjects.end())
		return nullptr;

	for (GameObject* object : totalObjects[key]) {
		if (!object->isActive)
			return object;
	}
	return nullptr;
}
