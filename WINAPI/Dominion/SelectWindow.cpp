#include "framework.h"
#include "Act.h"
#include "Card.h"
#include "SelectWindow.h"
#include "GameMaster.h"
#include "DominionPlayer.h"

SelectWindow::SelectWindow()
	: ImageRect(L"Textures/Dominion/Texture/SelectWindow.bmp")
{
	selectedResult = new GetCardResult();
	unselectedResult = new GetCardResult();

	cardRect.resize(80);

	for (int i = 0; i < cardRect.size(); i++) {
		cardRect[i] = { new ImageRect(), nullptr };
		cardRect[i].first->isActive = false;
	}

	isActive = false;

	Texture* tab = Texture::Add(L"Textures/Dominion/Texture/ScrollTab.bmp", 2, 1);

	leftTab = new ImageRect(tab);
	rightTab = new ImageRect(tab);
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

	delete leftTab;
	delete rightTab;
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

	this->selectableFunc = selectableFunc;	//선택 가능 검사 함수
	this->selectFunc = selectFunc;			//선택 시 처리 함수
	this->endFunc = endFunc;				//선택 종료 후 결과 처리 함수

	for (int i = 0; i < nRect; i++) {
		cardRect[i].second = request->cards[i];
		cardRect[i].first->isActive = true;
		cardRect[i].first->size = cardRect[i].second->size;
		request->cards[i]->SetSelectable(selectableFunc);
	}

	SortRects();

	this->covered = covered;

	waitTime = 0.0f;
}

void SelectWindow::SetResize(Vector2 pos, Vector2 size)
{
	this->pos = pos;
	this->size = size;

	for (int i = 0; i < cardRect.size(); i++) {
		cardRect[i].first->pos.y = pos.y;
	}

	leftTab->pos = { Left() + leftTab->Half().x, pos.y};
	rightTab->pos = { Right() - rightTab->Half().x, pos.y};
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
void SelectWindow::EndCall() 
{
	int cur = CurSelectedNum();
	if (CurSelectedNum() >= minNum) {
		endFunc(); 
		isEnd = true; 
	} 
}
void SelectWindow::Update()
{
	if (isEnd) {
		if (player->isAi && waitTime < waitRate) {
			//AI라면 바로 끝내지 말고 잠깐 보여주자
			waitTime += DELTA;
			return;
		}

		Done();
		return;
	}

	//선택 가능한 카드 표시
	for (int i = 0; i < nRect; i++) {
		cardRect[i].second->SetSelectable(selectableFunc);
	}

	if (player->isAi) {
		//AI : 앞에서부터 선택 가능한 것들 선택
		int cnt = 0;
		for (int i = 0; i < nRect; i++) {
			if (cardRect[i].second->IsSelectable() && !cardRect[i].second->IsSelected())
				selectFunc(cardRect[i].second);
			if (cardRect[i].second->IsSelected())
				cnt++;

			//다 골랐으면 끝
			if (cnt >= minNum) {
				EndCall();
				return;
			}
		}
	}
	else {
		//버튼 설정
		DominionGameMaster::Get()->SetEndButton("완료", bind(&SelectWindow::EndCall, this));
		MoveRects();
		
		if (KEY_DOWN(VK_LBUTTON)) {
			//눌린 카드 찾기
			auto card = GetCardMouseOn();
			if(card != nullptr && card->IsSelectable())
				selectFunc(card);
		}
	}
}

void SelectWindow::Render(HDC hdc)
{
	if (!isActive)
		return;

	__super::Render(hdc);

	for (int i = 0; i < nRect; i++) {
		if(cardRect[i].first->isActive && IsClickable(i))
			cardRect[i].second->Render(hdc, cardRect[i].first, covered);
	}

	leftTab->Render(hdc, { 1, 0 });
	rightTab->Render(hdc, { 0, 0 });

	SetTextColor(hdc, WHITE);
	TextOutA(hdc, (int)(pos.x - FONT_WIDTH / 2 * explain.size()), (int)Top() + 10, explain.c_str(), (int)explain.size());
	SetTextColor(hdc, BLACK);
}

int SelectWindow::CurSelectedNum()
{
	int result = 0;
	for (int i = 0; i < nRect; i++) {
		if (!cardRect[i].first->isActive)
			continue;

		if (cardRect[i].second->IsSelected())
			result++;
	}

	return result;
}

Card* SelectWindow::GetCardMouseOn()
{
	Card* result = nullptr;
	//유효한 카드들 중 커서 올라간 카드 찾기
	for (int i = 0; i < nRect; i++) {
		//애초에 유효하지 않은 카드. 오류
		if (!cardRect[i].first->isActive)
			continue;

		//출력 안 된 카드면 잡히지도 않는다
		if (!IsClickable(i))
			continue;

		//잡혔는지 확인
		if (cardRect[i].first->IsPointCollision(mousePos)) {
			result = cardRect[i].second;
		}
	}

	return result;
}

void SelectWindow::SortRects()
{
	float margin = 5.0f;
	float sum = margin + Left() + leftTab->size.x;

	for (int i = 0; i < nRect; i++) {
		cardRect[i].first->pos.x = sum + cardRect[i].first->size.x * 0.5f;
		sum += cardRect[i].first->size.x + margin;
	}
}

void SelectWindow::MoveRects()
{
	//활성화된 카드가 없다 => 움직일 카드가 없다
	if (nRect == 0)
		return;

	float move = 0.0f;
	//leftTab에 커서가 올라왔고 유효한 가장 왼쪽 카드가 leftTab에 걸리지 않았다
	if (leftTab->IsPointCollision(mousePos)	&& cardRect[0].first->Left() < Left() + leftTab->size.x) {
		move += 200.0f * DELTA;
	} 

	//rightTab에 커서가 올라왔고 유효한 가장 오른쪽 카드가 rightTab에 걸리지 않았다
	if (rightTab->IsPointCollision(mousePos) && cardRect[nRect-1].first->Right() > Right() - rightTab->size.x) {
		move -= 200.0f * DELTA;
	}

	if (abs(move) > 0.0f) {
		for (auto& rect : cardRect)
			rect.first->pos.x += move;
	}
}

bool SelectWindow::IsClickable(int i)
{
	//창에 출력되는 카드인지 검사 
	return (cardRect[i].first->Right() >= Left() + leftTab->size.x && cardRect[i].first->Left() <= Right() - rightTab->size.x);
}
