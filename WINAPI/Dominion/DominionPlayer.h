#pragma once
class Card;

class DominionPlayer
{
public:
	vector<Card*> deck;
	vector<Card*> hand;
	vector<Card*> used;
	vector<Card*> discard;

	void ReloadDeck();

};

