#include "framework.h"
#include "DominionPlayer.h"
#include "Card.h"
#include "CardDataManager.h"


DominionPlayer::DominionPlayer(bool isControl, bool isAi)
	: isController(isControl), isAi(isAi)
{
	deckRect = new Rect();
	handRect = new Rect();
	usedRect = new Rect();
	if (isController) {
		deckRect->pos = { 400.0f, WIN_HEIGHT - 60.0f };
		deckRect->size = { 80.0f, 120.0f };
		handRect->pos = { 700.0f, WIN_HEIGHT - 60.0f };
		handRect->size = {};
		usedRect->pos = { 700.0f, WIN_HEIGHT - 180.0f };
		usedRect->size = handRect->size;
	}
	else {
		deckRect->pos = { 400.0f, 60.0f };
		deckRect->size = { 80.0f, 120.0f };
		handRect->pos = { 700.0f, 60.0f };
		handRect->size = { 800.0f, 120.0f};
		usedRect->pos = { 700.0f, 180.0f };
		usedRect->size = handRect->size;
	}
}

DominionPlayer::~DominionPlayer()
{
	delete deckRect;
}

void DominionPlayer::ReloadDeck()
{
	for (int i = 0; i < discard.size() - 1; i++) {
		int p = Random(i, (int)discard.size() - 1);
		swap(discard[i], discard[p]);
	}

	while (!discard.empty()) {
		deck.push_back(discard.back());
		discard.pop_back();
	}
}

void DominionPlayer::InsertToDiscard(Card* card)
{
	card->isVisible = false;
	discard.push_back(card);
}

void DominionPlayer::Render(HDC hdc)
{

	auto texture = CardDataManager::Get()->texture;
	texture->Render(hdc, deckRect);
	wstring str = to_wstring(deck.size());
	TextOut(hdc, (int)deckRect->Left(), (int)deckRect->Top(), str.c_str(), (int)str.size());

	for (auto hCard : hand)
		hCard->Render(hdc);

	for (auto uCard : used)
		uCard->Render(hdc);
}

//턴 시작 시
void DominionPlayer::TurnStart()
{
	numAction = 1;
	numBuy = 1;
	gold = 0;
}

wstring DominionPlayer::GetInfo()
{
	return L"Action " + to_wstring(numAction)
		+ L" Buy " + to_wstring(numBuy) 
		+ L" Gold " + to_wstring(gold);
}
