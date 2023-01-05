#include "framework.h"
#include "Act.h"
#include "Card.h"
#include "SelectWindow.h"
#include "GameMaster.h"

SelectWindow::SelectWindow()
{
	selectedResult = new GetCardResult();
	unselectedResult = new GetCardResult();

	rect.resize(10);
	for (int i = 0; i < 10; i++) {
		rect[i] = { new ImageRect(), nullptr };
		rect[i].first->isActive = false;
	}
}

SelectWindow::~SelectWindow()
{
	delete selectedResult;
	delete unselectedResult;
	for (auto& r : rect) {
		delete r.first;
	}
	rect.clear();
	rect.shrink_to_fit();
}

void SelectWindow::Init(
	GetCardResult* request, 
	function<bool(CardData*)> selectableFunc, 
	function<void()> selectFunc, 
	function<void()> endFunc,
	bool covered)
{
	isEnd = false;

	nRect = (int)request->cards.size();

	this->selectableFunc = selectableFunc;
	this->selectFunc = selectFunc;
	this->endFunc = endFunc;

	

	for (int i = 0; i < nRect; i++) {
		rect[i].first->isActive = true;
		rect[i].second = request->cards[i];
		request->cards[i]->SetSelectable(selectableFunc);
	}

	this->covered = covered;
}

void SelectWindow::Update()
{
	if (isEnd) {
		return;
	}
	DominionGameMaster::Get()->endButton->SetEvent([this]() -> void { 
		endFunc(); 
		DominionGameMaster::Get()->endButton->SetEvent(nullptr); 
	});

	for (int i = 0; i < nRect; i++) {
		if (KEY_DOWN(VK_LBUTTON)) {
			if (rect[i].first->IsPointCollision(mousePos) && rect[i].second->IsSelectable()) {
				selectFunc();
			}
		}
	}
}

void SelectWindow::Render(HDC hdc)
{
	__super::Render(hdc);

	for (int i = 0; i < nRect; i++) {
		rect[i].second->Render(hdc, rect[i].first, covered);
	}
}
