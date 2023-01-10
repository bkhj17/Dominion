#pragma once
class Card;

class CardSet : public Rect
{
public:
	CardSet(bool isCovered, bool isOneSet);
	~CardSet();
	
	virtual void Update() {}
	virtual void Render(HDC hdc);

	void InputCard(Card* input, bool toTop = false, bool teleport = false);
	void InputCard(vector<Card*>& inputs, bool toTop = false, bool teleport = false);
	void InputCard(deque<Card*>& inputs, bool toTop = false, bool teleport = false);
	void InputCard(queue<Card*>& inputs, bool toTop = false, bool teleport = false);

	//ī�� �������� ������ ��ġ�� �̵�
	void SetCardPos(Card* card, int index, bool teleport);
	//��� ī�� ��ġ ����
	void SortCardPos();

	//ī�� ����
	void Shuffle();

	virtual Card* GetByPos(Vector2 pos);

	Card* Pop();
	Card* Out(Card* out);
	Card* Out(int n);

	//��� ī�� ���� ���� Ȯ��
	bool FindSelectable(function<bool(Card*)> condition);
	//��� ī�� ���� �Ұ� ���·�
	void SetUnselectable();

public:
	deque<Card*> cards;

	bool isVisible = true;		//���� ����
	bool isCovered;		//ī�� ��������
	bool isOneSet;		//�������� �ƴ��� ����
	bool frontRender = true;
};

