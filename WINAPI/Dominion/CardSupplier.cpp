#include "framework.h"
#include "Card.h"
#include "CardDataManager.h"
#include "CardManager.h"
#include "CardSupplier.h"
#include "GameMaster.h"

void CardSupplier::Init(int key, int num)
{
	this->num = num;
	this->data = &CardDataManager::Get()->datas[key];

	auto originSize = size;
	SetTexture(data->texture);
	size = originSize;

	CardManager::Get()->CreateObjects(key, num);
	selectablePen = CardManager::Get()->GetSelectablePen();
}

void CardSupplier::Render(HDC hdc)
{
	if (num == 0)
		__super::Render(hdc, 125, data->frame);
	else
		__super::Render(hdc, data->frame);
	
	if (selectablePen && selectable) {
		auto post = SelectObject(hdc, selectablePen);
		LineRender(hdc);
		SelectObject(hdc, post);
	}

	SetBkMode(hdc, 2);
	SetTextColor(hdc, num > 0 ? YELLOW : RED);

	wstring str = to_wstring(num);
	TextOut(hdc, (int)Left(), (int)Top(), str.c_str(), (int)str.size());

	SetTextColor(hdc, BLACK);
	SetBkMode(hdc, 0);
}

Card* CardSupplier::SupplyCard()
{
	if (num == 0)
		return nullptr;

	num--;
	Card* card = (Card*)CardManager::Get()->PopByIntKey(data->key);
	card->size = size;
	card->isActive = true;
	return card;
}

void CardSupplier::SetSelectable(function<bool(CardSupplier*)> condition)
{
	selectable = !Empty() && (condition ? condition(this) : false);
}
