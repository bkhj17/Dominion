#include "framework.h"
#include "SceneManager.h"

SceneManager::~SceneManager()
{
	for (auto& scene : scenes) {
		delete scene.second;
	}
}

void SceneManager::Update()
{
	if (curScene == nullptr)
		return;

	curScene->Update();
}

void SceneManager::Render(HDC hdc)
{
	if (curScene == nullptr)
		return;

	curScene->Render(hdc);
}

void SceneManager::Add(string key, Scene* scene)
{
	if (scenes.count(key) > 0)
		return;

	scenes[key] = scene;
}

void SceneManager::ChangeScene(string key)
{
	if (scenes.count(key) == 0)
		return;

	if (scenes[key] == curScene)
		return;

	if (curScene != nullptr)
		curScene->End();

	curScene = scenes[key];
	curScene->Start();
}
