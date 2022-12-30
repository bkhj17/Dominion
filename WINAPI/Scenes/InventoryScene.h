#pragma once
#include "Scene.h"
class InventoryScene : public Scene
{
public:
	InventoryScene();
	~InventoryScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	Store* store;
	Inventory* inventory;
};

