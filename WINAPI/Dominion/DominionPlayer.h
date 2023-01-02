#pragma once
class Card;

class DominionPlayer
{
public:
	DominionPlayer(bool isControl);
	~DominionPlayer();

	bool isController = false;
	
	vector<Card*> deck;
	Rect* deckRect;

	vector<Card*> hand;
	Rect* handRect;

	vector<Card*> used;
	Rect* usedRect;

	vector<Card*> discard;	//�� ������, deck�̶� pos ����


	void ReloadDeck();

	void InsertToDiscard(Card* card);

	void Render(HDC hdc);
};

