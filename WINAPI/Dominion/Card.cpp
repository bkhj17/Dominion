#include "framework.h"
#include "Act.h"
#include "MyMovement.h"
#include "Card.h"

CardData::CardData()
{
}

CardData::~CardData()
{
}

Card::Card(CardData data)
{
	movement = new MyPointMovement(this);
	this->data = new CardData(data);
	isActive = false;

	ImageRect::SetTexture(data.texture);
}

Card::~Card()
{
	delete data;
	delete movement;
}

void Card::Update()
{
	movement->Update();
}

void Card::Render(HDC hdc)
{
	if (!isVisible)
		return;

	__super::Render(hdc, isCovered ? data->covered : data->frame);

	if (isSelectable) {
		//선택 가능한 카드 표시
	}
}

int Card::GetVictory()
{
	if (SpecialVictory) {
		return SpecialVictory(data->key);
	}
	else {
		return data->victory;
	}
}
