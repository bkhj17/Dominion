#include "framework.h"
#include "Panel.h"

Panel::Panel(wstring file)
	: ImageRect(file)
{
	thumbnailImage = new ImageRect();
	thumbnailImage->pos = pos;
}

Panel::~Panel()
{
	for (auto item : items)
		delete item;
	items.clear();

	delete thumbnailImage;
}

void Panel::Update()
{
	Drag();
	for (auto item : items) {
		item->pos = item->LocalPos() + pos;
		item->Update();
	}
	thumbnailImage->pos = pos + thumbnailOffset;
}

void Panel::Render(HDC hdc)
{
	__super::Render(hdc);
	for (auto item : items) {
		item->Render(hdc);
	}
	thumbnailImage->Render(hdc);
	Vector2 descPos = pos + Vector2(-100, -85);
	TextOutA(hdc, descPos.x, descPos.y, descStr.c_str(), descStr.size());
}

void Panel::SelectItem(void* item)
{
	selectItem = (Item*)item;

	if (selectItem == nullptr)
		return;

	thumbnailImage->SetTexture(selectItem->GetTexture());
	thumbnailImage->size = { 120, 120 };

	descStr = "Name : " + selectItem->GetData().name + " ";
	descStr += "Price : " + selectItem->GetData().price;
}

void Panel::Drag()
{
	if (KEY_DOWN(VK_LBUTTON)) {
		if (IsPointCollision(mousePos))
		{
			isDrag = true;
			offset = pos - mousePos;
		}
	}

	if (isDrag && KEY_PRESS(VK_LBUTTON)) {
		pos = mousePos + offset;
	}

	if (KEY_UP(VK_LBUTTON)) {
		isDrag = false;
	}
}
