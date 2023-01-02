#pragma once
class CardManager : public PoolingManager, public Singleton<CardManager>
{
private:
	friend class Singleton;

public:
	void CreateObjects(int key, UINT poolSize);
	GameObject* PopByIntKey(int key);

};

