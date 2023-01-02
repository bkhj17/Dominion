#include "framework.h"
#include "Card.h"
#include "CardManager.h"
#include "CardSupplier.h"

CardSupplier::CardSupplier()
{
}

void CardSupplier::Init(CardData* data, int num)
{
	this->num = num;
	this->data = data;
	SetTexture(data->texture);

	CardManager::Get()->CreateObjects(data->key, num);
}

void CardSupplier::Render(HDC hdc)
{
	__super::Render(hdc, data->frame);

	wstring str = to_wstring(num);
	TextOut(hdc, Left(), Top(), str.c_str(), str.size());
}

Card* CardSupplier::SupplyCard()
{
	if (num == 0)
		return nullptr;

	num--;
	Card* card = (Card*)CardManager::Get()->Pop(data->name);
	return card;
}
