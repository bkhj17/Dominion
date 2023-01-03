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
	wstring GetInfo();

	bool isController = false;

	vector<Card*> deck;
	Rect* deckRect;

	vector<Card*> hand;
	Rect* handRect;

	vector<Card*> used;
	Rect* usedRect;

	vector<Card*> discard;	//�� ������, deck�̶� pos ����

	bool isAi = false;
	int numAction = 0;
	int numBuy = 0;
	int gold = 0;
};

