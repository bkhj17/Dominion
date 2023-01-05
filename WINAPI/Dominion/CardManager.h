#pragma once
class CardManager : public PoolingManager, public Singleton<CardManager>
{
private:
	friend class Singleton;

	CardManager();
	~CardManager();

public:
	void CreateObjects(int key, UINT poolSize);
	GameObject* PopByIntKey(int key);

	HPEN GetSelectablePen() { return selectablePen; }
	HPEN GetSelectedPen() { return selectedPen; }
private:
	HPEN selectablePen;
	HPEN selectedPen;
};

