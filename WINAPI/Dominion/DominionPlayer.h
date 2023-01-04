#pragma once
class Card;
class CardSet;

class DominionPlayer
{
public:
	DominionPlayer(bool isControl, bool isAi);
	~DominionPlayer();

	void ReloadDeck();
	void InsertToDiscard(Card* card);
	void Render(HDC hdc);

	void TurnStart();
	wstring GetInfo();

	bool isController = false;

	CardSet* deck;

	CardSet* hand;

	CardSet* used;
//	vector<Card*> used;
//	Rect* usedRect;
	CardSet* discard;

	bool isAi = false;
	int numAction = 0;
	int numBuy = 0;
	int gold = 0;
};

