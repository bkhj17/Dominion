#include "framework.h"
#include "PoolingManager.h"

PoolingManager::~PoolingManager()
{
	for (pair<string, Objects> objects : totalObjects) {
		for (GameObject* object : objects.second)
			delete object;
		objects.second.clear();
	}
	totalObjects.clear();
}

void PoolingManager::Update()
{
	for (pair<string, Objects> objects : totalObjects) {
		for (GameObject* object : objects.second)
			if(object->isActive)
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

void PoolingManager::ClearObjects(string key)
{
	if (totalObjects.count(key) == 0)
		return;

	for (GameObject* object : totalObjects[key]) {
		delete object;
	}
	totalObjects[key].clear();
	totalObjects.erase(key);
}
