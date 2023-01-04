#pragma once
class Card;

class CardSet : public Rect
{
public:
	CardSet(bool isCovered, bool isOneSet);
	~CardSet();
	
	void Render(HDC hdc);

	void InputCard(Card* input, bool teleport = false);
	void InputCard(vector<Card*>& inputs, bool teleport = false);
	void InputCard(deque<Card*>& inputs, bool teleport = false);
	void InputCard(queue<Card*>& inputs, bool teleport = false);

	void SetCardPos(Card* card, int index, bool teleport);

	void Shuffle();

	Card* GetByPos(Vector2 pos);

	Card* Pop();
	Card* Out(Card* out);
	Card* Out(int n);

	deque<Card*> cards;

	bool isVisible = true;		//���� ����
	bool isCovered;		//ī�� ��������
	bool isOneSet;		//�������� �ƴ��� ����
};

