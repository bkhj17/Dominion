#pragma once
class Card;

class CardSet : public Rect
{
public:
	CardSet(bool isCovered, bool isOneSet);
	~CardSet();
	
	void Render(HDC hdc);

	void InputCard(Card* input, bool toTop = false, bool teleport = false);
	void InputCard(vector<Card*>& inputs, bool toTop = false, bool teleport = false);
	void InputCard(deque<Card*>& inputs, bool toTop = false, bool teleport = false);
	void InputCard(queue<Card*>& inputs, bool toTop = false, bool teleport = false);

	void SetCardPos(Card* card, int index, bool teleport);
	void SortCardPos();

	void Shuffle();

	Card* GetByPos(Vector2 pos);

	Card* Pop();
	Card* Out(Card* out);
	Card* Out(int n);

	bool FindSelectable(function<bool(Card*)> condition);
	void SetUnselectable();

public:
	deque<Card*> cards;

	bool isVisible = true;		//보임 여부
	bool isCovered;		//카드 공개여부
	bool isOneSet;		//묶음인지 아닌지 여부
	bool frontRender = true;
	Vector2 cardSize;
};

