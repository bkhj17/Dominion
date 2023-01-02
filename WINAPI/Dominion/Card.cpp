#include "framework.h"
#include "Act.h"
#include "MyMovement.h"
#include "Card.h"

CardData::CardData()
{
	type.resize((int)(CardType::END) - 1);
}

CardData::~CardData()
{
	type.clear();
	text.clear();
	acts.clear();
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
	__super::Render(hdc, isCovered ? data->covered : data->frame);

	if (isSelectable) {
		//���� ������ ī�� ǥ��
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