#include "framework.h"
#include "MyMovement.h"
#include "Card.h"
#include "CardSet.h"
#include "CardDataManager.h"

CardSet::CardSet(bool isCovered, bool isOneSet)
	: isCovered(isCovered), isOneSet(isOneSet)
{
	size = Card::DEFAULT_SIZE;
}

CardSet::~CardSet()
{
	cards.clear();
}

void CardSet::Render(HDC hdc)
{
	if (!isVisible)
		return;

	if (isOneSet && isCovered) {
		CardDataManager::Get()->RenderCovered(hdc, this);
		return;
	}

	
	if (cards.empty()) {
		return;
	}

	if (isOneSet) {
		if (frontRender) {
			cards.front()->Render(hdc);
			wstring str = to_wstring(cards.size());
			TextOut(hdc, (int)Left(), (int)Top(), str.c_str(), (int)str.size());
		}
		else {
			cards.back()->Render(hdc);
			wstring str = to_wstring(cards.size());
			TextOut(hdc, (int)Left(), (int)Top(), str.c_str(), (int)str.size());
		}
	}
	else {
		for (auto card : cards)
			card->Render(hdc);
	}	
}

void CardSet::InputCard(Card* input, bool toTop, bool teleport)
{
	input->size = size;
	input->isVisible = isVisible;
	input->isCovered = isCovered;

	input->SetSelectable(nullptr);
	if (toTop) {
		cards.push_front(input);
	} else
		cards.push_back(input);
	SortCardPos();
}

void CardSet::InputCard(vector<Card*>& inputs, bool toTop, bool teleport)
{
	for (auto input : inputs) {
		input->size = size;
		input->isVisible = isVisible;
		input->isCovered = isCovered;

		input->SetSelectable(nullptr);
		if (toTop)
			cards.push_front(input);
		else
			cards.push_back(input);

	}
	inputs.clear();
	SortCardPos();
}

void CardSet::InputCard(deque<Card*>& inputs, bool toTop, bool teleport)
{
	while (!inputs.empty()) {
		Card* input = inputs.front();
		input->size = size;
		input->isVisible = isVisible;
		input->isCovered = isCovered;
		input->SetSelectable(nullptr);
		inputs.pop_front();
		if (toTop)
			cards.push_front(input);
		else
			cards.push_back(input);
	}
	SortCardPos();
}

void CardSet::InputCard(queue<Card*>& inputs, bool toTop, bool teleport)
{
	while (!inputs.empty()) {
		Card* input = inputs.front();
		inputs.pop();
		input->size = size;
		input->isVisible = isVisible;
		input->isCovered = isCovered;
		input->SetSelectable(nullptr);
		if (toTop)
			cards.push_front(input);
		else
			cards.push_back(input);

	}
	SortCardPos();
}

void CardSet::SetCardPos(Card* card, int index, bool teleport)
{	
	Vector2 newPos;
	if (isOneSet) {
		newPos = pos;
	}
	else {
		newPos = { Left() + card->size.x * (0.5f + index), pos.y };
	}

	if (teleport)
		card->movement->Teleport(newPos);
	else
		card->movement->SetTargetPosByTime(newPos, 0.2f);
}

void CardSet::SortCardPos()
{
	for (int i = 0; i < cards.size(); i++) {
		SetCardPos(cards[i], i, false);
	}
}

void CardSet::Shuffle()
{
	for (int i = 0; i < cards.size() - 1; i++) {
		int p = Random(i, (int)cards.size() - 1);
		swap(cards[i], cards[p]);
	}
}

Card* CardSet::GetByPos(Vector2 pos)
{
	Card* result = nullptr;
	for (auto c : cards) {
		if (c->IsPointCollision(pos)) {
			result = c;
			//카드가 겹치는 경우 제일 위에 있는 카드를 집어야 하기 때문에 바로 끊지 않는다
		}
		else if (result != nullptr) {
			//근데 잡힌게 있는데 안 잡히면 이상하지
			break;
		}
	}

	return result;
}

Card* CardSet::Pop()
{
	if(cards.empty())
		return nullptr;

	Card* out = cards.front();
	cards.pop_front();

	SortCardPos();
	return out;
}

Card* CardSet::Out(Card* out)
{
	Card* result = nullptr;
	for (auto it = cards.begin(); it != cards.end(); it++) {
		if (out == *it) {
			result = *it;
			cards.erase(it);
			break;
		}
	}

	SortCardPos();
	return result;
}

Card* CardSet::Out(int n)
{
	if(cards.size() <= n)
		return nullptr;

	Card* out = cards[n];
	cards.erase(cards.begin() + n);
	SortCardPos();
	return out;
}

bool CardSet::FindSelectable(function<bool(Card*)> condition)
{
	bool result = false;

	for (auto card : cards) {
		card->SetSelectable(condition);
		result |= card->IsSelectable();
	}

	return result;
}

void CardSet::SetUnselectable()
{
	for (auto card : cards) {
		card->SetSelected(false);
		card->SetSelectable(nullptr);
	}
}