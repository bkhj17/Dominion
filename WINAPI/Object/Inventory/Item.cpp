#include "framework.h"
#include "Item.h"

Item::Item(ItemData data)
	: Button(data.file), data(data)
{
	SetEvent(bind(&Item::ClickStoreItem, this));
}

Item::~Item()
{
}

void Item::Update()
{
	__super::Update();
}

void Item::Render(HDC hdc)
{
	__super::Render(hdc);
}

void Item::ClickStoreItem()
{
	Observer::Get()->ExcuteParamEvents("StoreItemSelect", this);
}

void Item::ClickInventoryItem()
{
	Observer::Get()->ExcuteParamEvents("InventoryItemSelect", this);
}
