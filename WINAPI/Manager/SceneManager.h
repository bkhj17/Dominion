#pragma once
class SceneManager : public Singleton<SceneManager>
{
private:
	friend class Singleton;
	SceneManager() = default;
	~SceneManager();
public:
	void Update();
	void Render(HDC hdc);

	void Add(string key, Scene* scene);

	void ChangeScene(string key);
private:
	map<string, Scene*> scenes;

	Scene* curScene = nullptr;
};

