#include "framework.h"
#include "Inventory.h"

Inventory::Inventory()
	: Panel(L"Textures/Inventory/Panal.bmp")
{
	Observer::Get()->AddParamEvent(
		"PurchaseItem", 
		bind(&Inventory::PurchaseItem, this, placeholders::_1));

	Observer::Get()->AddEvent(
		"SellItem",
		bind(&Inventory::SellItem, this));

	Observer::Get()->AddParamEvent(
		"InventoryItemSelect",
		bind(&Panel::SelectItem, this, placeholders::_1));
}

Inventory::~Inventory()
{
}

void Inventory::Update()
{
	__super::Update();
}

void Inventory::Render(HDC hdc)
{
	__super::Render(hdc);

	Vector2 goldTextPos = pos + goldPos;
	auto str = to_wstring(gold);
	TextOut(hdc, goldTextPos.x, goldTextPos.y, str.c_str(), str.size());
}

void Inventory::PurchaseItem(void* item)
{
	//구매
	Item* pItem = reinterpret_cast<Item*>(item);

	if (pItem == nullptr)
		return;

	ItemData data = pItem->GetData();

	if (gold < data.price) {
		MessageBox(hWnd, L"너 돈 없어", L"Fail", MB_OK);
	}
	else {
		gold -= data.price;
		Item* newItem = new Item(data);
		newItem->size = { 75, 75 };
		newItem->SetEvent(bind(&Item::ClickInventoryItem, newItem));
		items.push_back(newItem);
		SetItemPos(items.size() - 1);
	}
}

void Inventory::SellItem()
{
	if (selectItem == nullptr)
		return;

	gold += selectItem->GetData().price;
	for (auto it = items.begin(); it != items.end(); it++) {
		if (selectItem == *it) {
			items.erase(it);
			delete selectItem;
			selectItem = nullptr;
		}
	}
}

void Inventory::SetItemPos(int i) {
	int colCount = 5;
	float interval = 10.0f;
	Vector2 startOffset(-255, 40);

	Vector2 coord = Vector2(
		i % colCount,
		i / colCount
	);

	items[i]->LocalPos() = startOffset + offset + coord * interval;
	items[i]->LocalPos().x += coord.x * items[i]->size.x;
	items[i]->LocalPos().y += coord.y * items[i]->size.y;
}

void Inventory::SetItemPos()
{
	for (int i = 0; i < items.size(); i++)
		SetItemPos(i);
}
