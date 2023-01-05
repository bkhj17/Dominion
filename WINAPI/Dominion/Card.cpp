#include "framework.h"
#include "Act.h"
#include "MyMovement.h"
#include "Card.h"

const Vector2 Card::DEFAULT_SIZE = { 60, 90 };

Card::Card(CardData data)
{
	movement = new MyPointMovement(this);
	this->data = new CardData(data);
	isActive = false;
	size = { 60.0f, 90.0f };

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

	Render(hdc, this, isCovered);
}

void Card::Render(HDC hdc, Rect* rect, bool covered)
{
	//visible 여부에 상관 없이 정해진 위치에 띄우기
	texture->Render(hdc, rect, covered ? data->covered : data->frame);

	if (isSelected) {
		auto post = SelectObject(hdc, selectedPen);
		rect->LineRender(hdc);
		SelectObject(hdc, post);
	}
	else if (isSelectable) {
		//선택 가능한 카드 표시
		auto post = SelectObject(hdc, selectablePen);
		rect->LineRender(hdc);
		SelectObject(hdc, post);
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

void Card::SetSelectable(function<bool(CardData*)> condition)
{
	isSelectable = condition ? condition(data) : false;
	this->isSelected = false;
}
