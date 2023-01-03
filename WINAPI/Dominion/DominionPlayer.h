#pragma once
class Card;

class DominionPlayer
{
public:
	DominionPlayer(bool isControl, bool isAi);
	~DominionPlayer();

	void ReloadDeck();
	void InsertToDiscard(Card* card);
	void Render(HDC hdc);

	void TurnStart();

	bool isController = false;

	vector<Card*> deck;
	Rect* deckRect;

	vector<Card*> hand;
	Rect* handRect;

	vector<Card*> used;
	Rect* usedRect;

	vector<Card*> discard;	//¾È º¸¿©ÁÜ, deckÀÌ¶û pos °øÀ¯

	wstring GetInfo();

	bool isAi = false;
	int numAction = 0;
	int numBuy = 0;
	int gold = 0;
};

