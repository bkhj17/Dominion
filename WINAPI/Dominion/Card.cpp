#include "framework.h"
#include "Act.h"
#include "MyMovement.h"
#include "Card.h"

const Vector2 Card::DEFAULT_SIZE = { 60, 90 };

Card::Card(CardData* data)
{
	movement = new MyPointMovement(this);
	this->data = data;
	isActive = false;
	size = { 60.0f, 90.0f };

	ImageRect::SetTexture(data->texture);

	
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
	if (!isActive || !isVisible)
		return;

	//������

	Render(hdc, this, isCovered);
}

void Card::Render(HDC hdc, Rect* cardRect, bool covered)
{
	if (!isActive || !isVisible)
		return;

	//visible ���ο� ��� ���� ������ ��ġ�� ����
	
	texture->Render(hdc, cardRect, covered ? data->covered : data->frame);

	if (isSelected) {
		//���õ� ī�� ǥ��
		auto post = SelectObject(hdc, selectedPen);
		cardRect->LineRender(hdc);
		SelectObject(hdc, post);
	} else if (isSelectable) {
		//���� ������ ī�� ǥ��
		auto post = SelectObject(hdc, selectablePen);
		cardRect->LineRender(hdc);
		SelectObject(hdc, post);
	}
}

void Card::SetSelectable(function<bool(Card*)> condition)
{
	
	isSelectable = isSelected || ( condition ? condition(this) : false);
}
