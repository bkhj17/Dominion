#pragma once
class Card;
class CardSet;

class DominionPlayer
{
public:
	DominionPlayer(bool isControl, bool isAi);
	~DominionPlayer();

	void ReloadDeck();
	void Render(HDC hdc);

	void TurnStart();
	wstring GetInfo();

	bool IsController() { return isController; }

	void CalcScore();
	int GetScore() { return score; }
	
public:
	CardSet* deck;

	CardSet* hand;

	CardSet* used;

	CardSet* discard;

	bool isAi = false;
	int numAction = 0;
	int numBuy = 0;
	int gold = 0;

	string name;

private:
	bool isController = false;
	int score = 0;
};

