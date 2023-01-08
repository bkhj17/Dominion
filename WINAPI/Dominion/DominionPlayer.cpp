#include "framework.h"
#include "DominionPlayer.h"
#include "Card.h"
#include "CardSet.h"
#include "ScrollableCardSet.h"
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

		hand = new ScrollableCardSet(450.0f, false, isController);
		((ScrollableCardSet*)hand)->SetPos({ 700.0f, WIN_HEIGHT - 60.0f });
		hand->size = size;

		used = new ScrollableCardSet(350.0f, false, isController); 
		((ScrollableCardSet*)used)->SetPos({ 700.0f, WIN_HEIGHT - 180.0f });
		used->size = size;

		discard = new CardSet(false, true);
		discard->pos = { deck->pos.x + size.x, deck->pos.y };
		discard->size = size;
		discard->frontRender = false;
	}
	else {
		deck = new CardSet(true, true);
		deck->pos = { 400.0f, 60.0f };
		deck->size = size;

		hand = new ScrollableCardSet(450.0f, false, isController);
		hand->size = size;
		((ScrollableCardSet*)hand)->SetPos({ 700.0f, 60.0f });

		used = new ScrollableCardSet(350.0f, false, isController);
		((ScrollableCardSet*)used)->SetPos({ 700.0f, 180.0f });
		used->size = size;

		discard = new CardSet(false, true);
		discard->pos = { deck->pos.x + size.x, deck->pos.y };
		discard->size = size;
		discard->frontRender = false;
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

void DominionPlayer::Update()
{
	hand->Update();
	used->Update();
}

void DominionPlayer::Render(HDC hdc)
{
	deck->Render(hdc);
	hand->Render(hdc);
	used->Render(hdc);
	discard->Render(hdc);
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

void DominionPlayer::CalcScore()
{
	score = 0;
	for (auto card : deck->cards) {
		if (card->data->sVictory)
			score += CardDataManager::Get()->sVictoryFunc[card->data->key](this);
		else 
			score += card->data->victory;
	}

	for (auto card : hand->cards) {
		if (card->data->sVictory)
			score += CardDataManager::Get()->sVictoryFunc[card->data->key](this);
		else
			score += card->data->victory;
	}

	for (auto card : used->cards) {
		if (card->data->sVictory)
			score += CardDataManager::Get()->sVictoryFunc[card->data->key](this);
		else
			score += card->data->victory;
	}

	for (auto card : discard->cards) {
		if (card->data->sVictory)
			score += CardDataManager::Get()->sVictoryFunc[card->data->key](this);
		else
			score += card->data->victory;
	}
}
