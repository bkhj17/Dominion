#pragma once
class PoolingManager
{
public:
	~PoolingManager();

	virtual void Update();
	virtual void Render(HDC hdc);
	GameObject* Pop(string key);

	virtual void CreateObjects(string key, UINT poolSize) {};

	void ClearObjects(string key);
protected:
	typedef vector<GameObject*> Objects;

	map<string, Objects> totalObjects;
};

