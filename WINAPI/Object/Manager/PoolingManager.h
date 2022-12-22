#pragma once
class PoolingManager
{
public:
	~PoolingManager();

	void Update();
	void Render(HDC hdc);
	GameObject* Pop(string key);

	virtual void CreateObjects(string key, UINT poolSize) = 0; 

protected:
	typedef vector<GameObject*> Objects;

	map<string, Objects> totalObjects;
};

