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

	//카드 순서별로 정해진 위치로 이동
	void SetCardPos(Card* card, int index, bool teleport);
	//모든 카드 위치 정렬
	void SortCardPos();

	//카드 섞기
	void Shuffle();

	virtual Card* GetByPos(Vector2 pos);

	Card* Pop();
	Card* Out(Card* out);
	Card* Out(int n);

	//모든 카드 선택 여부 확인
	bool FindSelectable(function<bool(Card*)> condition);
	//모든 카드 선택 불가 상태로
	void SetUnselectable();

public:
	deque<Card*> cards;

	bool isVisible = true;		//보임 여부
	bool isCovered;		//카드 공개여부
	bool isOneSet;		//묶음인지 아닌지 여부
	bool frontRender = true;
};

