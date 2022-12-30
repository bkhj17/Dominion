#include "framework.h"
#include "InventoryScene.h"

InventoryScene::InventoryScene()
{
	DataManager::Get();
	store = new Store();
	store->pos = { CENTER_X, CENTER_Y - 200.0f };
	store->size *= 0.5;

	inventory = new Inventory();
	inventory->pos = { CENTER_X, CENTER_Y + 200.0f };
	inventory->size *= 0.5;
}

InventoryScene::~InventoryScene()
{
	DataManager::Delete();
	delete store;
	delete inventory;
}

void InventoryScene::Update()
{
	store->Update();
	inventory->Update();
}

void InventoryScene::Render(HDC hdc)
{
	store->Render(hdc);
	inventory->Render(hdc);
}
