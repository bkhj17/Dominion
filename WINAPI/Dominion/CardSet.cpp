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
		
	if (cards.empty() && !isCovered) {
		return;
	}

	if (isOneSet) {
		if (isCovered) {
			if(cards.empty())
				//카드 없으면 반투명
				CardDataManager::Get()->RenderCovered(hdc, this, 100);
			else
				CardDataManager::Get()->RenderCovered(hdc, this);
		}
		else if (frontRender)
			cards.front()->Render(hdc);
		else
			cards.back()->Render(hdc);

		//카드 수량 출력
		auto postBk = SetBkMode(hdc, 2);
		auto postColor = SetTextColor(hdc, cards.empty() ? WHITE : YELLOW);

		wstring str = to_wstring(cards.size());
		TextOut(hdc, (int)Left(), (int)Top(), str.c_str(), (int)str.size());

		SetTextColor(hdc, postColor);
		SetBkMode(hdc, postBk);
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
	for (int i = 0; i < cards.size(); i++)
		SetCardPos(cards[i], i, false);
}

void CardSet::Shuffle()
{
	for (int i = 0; i < cards.size() - 1; i++) {
		int p = Random(i, (int)cards.size() - 1);
		swap(cards[i], cards[p]);
	}

	SortCardPos();
}

Card* CardSet::GetByPos(Vector2 pos)
{
	Card* result = nullptr;
	if (isOneSet) {
		//쌓은 카드셋일 경우 위 혹은 아래 1장 출력
		if (this->IsPointCollision(pos) && !cards.empty())
			result = frontRender ? cards.front() : cards.back();
	} else {
		for (auto c : cards) {
			//커서가 카드 위에 있고 
			if (c->IsMouseOn() && !c->isCovered) {
				//카드가 겹치는 경우 제일 위에 있는 카드를 집어야 하기 때문에 바로 끊지 않는다
				result = c;
			}
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
	for (int i = 0; i < cards.size(); i++) {
		if (cards[i] == out)
			Out(i);
	}

	return result;
}

Card* CardSet::Out(int n)
{
	if(cards.size() <= n)
		return nullptr;

	Card* out = cards[n];
	cards.erase(cards.begin() + n);

	//남은 카드들 재배치
	SortCardPos();
	return out;
}

bool CardSet::FindSelectable(function<bool(Card*)> condition)
{
	bool result = false;

	//condition에 따라 카드들 선택 가능 여부 설정
	for (auto card : cards) {
		card->SetSelectable(condition);
		result |= card->IsSelectable();
	}

	//선택 가능한 카드가 존재했는지 여부 반환
	return result;
}

void CardSet::SetUnselectable()
{
	//모든 카드 선택 해제
	for (auto card : cards) {
		card->SetSelected(false);
		card->SetSelectable(nullptr);
	}
}