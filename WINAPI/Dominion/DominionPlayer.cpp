#include "framework.h"
#include "DominionPlayer.h"
#include "Card.h"
#include "CardSet.h"
#include "MyMovement.h"
#include "CardDataManager.h"


DominionPlayer::DominionPlayer(bool isControl, bool isAi)
	: isController(isControl), isAi(isAi)
{
	Vector2 size = { 80.0f, 120.0f };
	if (isController) {
		deck = new CardSet(true, true);
		deck->pos = { 400.0f, WIN_HEIGHT - 60.0f };
		deck->size = size;

		hand = new CardSet(false, false);
		hand->pos = { 700.0f, WIN_HEIGHT - 60.0f };
		hand->size = size;

		used = new CardSet(false, false);
		used->pos = { 700.0f, WIN_HEIGHT - 180.0f };
		used->size = size;

		discard = new CardSet(true, true);
		discard->pos = deck->pos;
		discard->size = size;
	}
	else {
		deck = new CardSet(true, true);
		deck->pos = { 400.0f, 60.0f };
		deck->size = size;

		hand = new CardSet(true, false);
		hand->pos = { 700.0f, 60.0f };
		hand->size = size;

		used = new CardSet(false, false);
		used->pos = { 700.0f, 180.0f };
		used->size = size;

		discard = new CardSet(true, true);
		discard->pos = deck->pos;
		discard->size = size;
	}
}

DominionPlayer::~DominionPlayer()
{
	delete deck;
	delete hand;
	delete used;
	delete discard;
}

void DominionPlayer::ReloadDeck()
{
	discard->Shuffle();
	deck->InputCard(discard->cards);
}

void DominionPlayer::Render(HDC hdc)
{
	deck->Render(hdc);

	hand->Render(hdc);

	used->Render(hdc);
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
