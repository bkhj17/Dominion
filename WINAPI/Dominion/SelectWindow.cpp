#include "framework.h"
#include "Act.h"
#include "Card.h"
#include "SelectWindow.h"
#include "GameMaster.h"
#include "DominionPlayer.h"

SelectWindow::SelectWindow()
{
	selectedResult = new GetCardResult();
	unselectedResult = new GetCardResult();

	cardRect.resize(30);


	for (int i = 0; i < cardRect.size(); i++) {
		cardRect[i] = { new ImageRect(), nullptr };
		cardRect[i].first->isActive = false;
	}

	isActive = false;
}

SelectWindow::~SelectWindow()
{
	delete selectedResult;
	delete unselectedResult;
	for (auto& r : cardRect) {
		delete r.first;
	}
	cardRect.clear();
	cardRect.shrink_to_fit();
}

void SelectWindow::Init(
	DominionPlayer* player,
	GetCardResult* request, 
	GetCardResult* selected,
	GetCardResult* unselected,
	function<bool(Card*)> selectableFunc, 
	function<void(Card*)> selectFunc, 
	function<void()> endFunc,
	bool covered)
{
	isActive = true;
	isEnd = false;

	this->player = player;

	this->selectedResult = selected;
	this->unselectedResult = unselected;

	nRect = (int)request->cards.size();

	this->selectableFunc = selectableFunc;
	this->selectFunc = selectFunc;
	this->endFunc = endFunc;

	float margin = 5.0f ;
	float sum = margin + Left();

	for (int i = 0; i < nRect; i++) {
		cardRect[i].second = request->cards[i];

		cardRect[i].first->isActive = true;
		cardRect[i].first->size = cardRect[i].second->size;
		cardRect[i].first->pos.x = sum + cardRect[i].first->size.x * 0.5f;
		sum += cardRect[i].first->size.x + margin;

		request->cards[i]->SetSelectable(selectableFunc);
	}

	this->covered = covered;
}

void SelectWindow::SetResize(Vector2 pos, Vector2 size)
{
	this->pos = pos;
	this->size = size;

	for (int i = 0; i < 10; i++) {
		cardRect[i].first->pos.y = pos.y;
		cardRect[i].first->isActive = false;
	}
}

void SelectWindow::Done()
{
	//버튼 비활성화
	DominionGameMaster::Get()->OffEndButton();

	for (auto& r : cardRect) {
		if (r.second != nullptr) {
			r.second->SetSelected(false);
			r.second->SetSelectable(nullptr);
			r.second = nullptr;
		}
		r.first->isActive = false;
	}
	nRect = 0;

	//연결 끊기
	request = nullptr;
	selectedResult = nullptr;
	unselectedResult = nullptr;
	//비활성화
	isActive = false;
	isEnd = false;
}

void SelectWindow::Update()
{
	if (isEnd) {
		Done();
		return;
	}

	for (int i = 0; i < nRect; i++) {
		cardRect[i].second->SetSelectable(selectableFunc);
	}
	if (player->isAi) {
		int cnt = 0;
		for (int i = 0; i < nRect; i++) {
			if (cardRect[i].second->IsSelectable() && !cardRect[i].second->IsSelected())
				selectFunc(cardRect[i].second);

			if (cardRect[i].second->IsSelected())
				cnt++;

			if (cnt >= minNum) {
				EndCall();
				return;
			}
		}
	}
	else {
		//버튼 설정
		DominionGameMaster::Get()->SetEndButton("완료", bind(&SelectWindow::EndCall, this));
		if (KEY_DOWN(VK_LBUTTON)) {
			//눌린 카드 찾기
			for (int i = 0; i < nRect; i++) {
				if (cardRect[i].first->IsPointCollision(mousePos) && cardRect[i].second->IsSelectable())
					selectFunc(cardRect[i].second);
			}
		}
	}
}

void SelectWindow::Render(HDC hdc)
{
	if (!isActive)
		return;

	__super::Render(hdc);

	for (int i = 0; i < nRect; i++) {
		if(cardRect[i].first->isActive)
			cardRect[i].second->Render(hdc, cardRect[i].first, covered);
	}
}

int SelectWindow::CurSelectedNum()
{
	int result = 0;
	for (int i = 0; i < cardRect.size(); i++) {
		if (!cardRect[i].first->isActive)
			continue;

		if (cardRect[i].second->IsSelected())
			result++;
	}

	return result;
}

CardData* SelectWindow::GetMouseOn()
{
	CardData* result = nullptr;
	for (int i = 0; i < cardRect.size(); i++) {
		if (!cardRect[i].first->isActive)
			continue;

		if (cardRect[i].first->IsPointCollision(mousePos)) {
			result = cardRect[i].second->data;
		}
		else if (result != nullptr)
			break;
	}

	return result;
}
