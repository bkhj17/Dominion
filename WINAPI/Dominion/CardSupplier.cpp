#include "framework.h"
#include "Card.h"
#include "CardDataManager.h"
#include "CardManager.h"
#include "CardSupplier.h"

CardSupplier::CardSupplier()
{
}

void CardSupplier::Init(int key, int num)
{
	this->num = num;
	this->data = &CardDataManager::Get()->datas[key];
	SetTexture(data->texture);

	CardManager::Get()->CreateObjects(key, num);
}

void CardSupplier::Render(HDC hdc)
{
	__super::Render(hdc, data->frame);

	wstring str = to_wstring(num);
	TextOut(hdc, (int)Left(), (int)Top(), str.c_str(), (int)str.size());
}

Card* CardSupplier::SupplyCard()
{
	if (num == 0)
		return nullptr;

	num--;
	Card* card = (Card*)CardManager::Get()->Pop(data->name);
	return card;
}
