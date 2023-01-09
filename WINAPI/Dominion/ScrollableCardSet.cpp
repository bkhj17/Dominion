#include "framework.h"
#include "CardSet.h"
#include "ScrollableCardSet.h"
#include "Card.h"
#include "MyMovement.h"

ScrollableCardSet::ScrollableCardSet(float xLength, bool isCovered, bool control)
	: CardSet(isCovered, false), xLength(xLength), isControlable(control)
{
	Texture* button = Texture::Add(L"Textures/Dominion/Texture/ScrollTab.bmp", 2, 1);


	leftTab = new ImageRect(button);
	rightTab = new ImageRect(button);
	rightTab->size = leftTab->size = { 50.0f, 50.0f };

	leftPos.x = -leftTab->size.x;
	rightPos.x = xLength + rightTab->size.x;
}

ScrollableCardSet::~ScrollableCardSet()
{
	delete leftTab;
	delete rightTab;
}

void ScrollableCardSet::Update()
{
	if(isControlable)
		Scroll();
}

void ScrollableCardSet::Render(HDC hdc)
{
	if (cards.empty())
		return;

	bool isMoving = false;
	for (auto card : cards) {
		if (card->Right() < Left() || card->Left() > Right() + xLength)
			continue;

		card->Render(hdc);

		isMoving |= card->movement->IsMoving();
	}

	if (isControlable && !isMoving) {
		if (!RightEnd())
			leftTab->Render(hdc, {1, 0}, false);
		if (!LeftEnd())
			rightTab->Render(hdc, { 0, 0 }, false);
	}
}

void ScrollableCardSet::Scroll()
{
	Vector2 move;

	if (leftTab->IsPointCollision(mousePos)) {
		if (!RightEnd())
			move.x -= scrollSpeed * DELTA;
	}
	 
	if (rightTab->IsPointCollision(mousePos)) {
		if(!LeftEnd())
			move.x += scrollSpeed * DELTA;
	}

	for (auto card : cards) {
		if(!card->movement->IsMoving())
			card->movement->SetTargetPos(card->pos + move);
	}
}

Card* ScrollableCardSet::GetByPos(Vector2 pos)
{
	Card* result = nullptr;
	for (auto card : cards) {
		if (card->Right() < Left() || card->Left() > Right() + xLength)
			continue;

		if (card->IsPointCollision(pos)) {
			if (card->isActive && card->isVisible && !card->isCovered)
				result = card;
		}
	}

	return result;
}

void ScrollableCardSet::SetPos(Vector2 pos)
{
	this->pos = pos;
	leftTab->pos = { leftPos.x + Left(), pos.y };
	rightTab->pos = { rightPos.x + Right(), pos.y };
}

bool ScrollableCardSet::LeftEnd()
{
	return !cards.empty() && cards.front()->Left() >= Left();
}

bool ScrollableCardSet::RightEnd()
{
	return !cards.empty() && cards.back()->Right() <= Right() + xLength;
}


