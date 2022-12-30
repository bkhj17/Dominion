#pragma once
#include "Scene.h"
class InventoryScene : public Scene
{
public:
	InventoryScene();
	~InventoryScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	Store* store;
	Inventory* inventory;
};

