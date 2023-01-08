#pragma once

class ScrollableCardSet : public CardSet
{
public:
	ScrollableCardSet(float xLength, bool isCovered, bool control);
	~ScrollableCardSet();

	void Update() override;
	void Render(HDC hdc) override;

	void Scroll();

	Card* GetByPos(Vector2 pos);

	void SetPos(Vector2 pos);

private:
	bool LeftEnd();
	bool RightEnd();
	//목록 돌려보기 가능한 카드셋
	//카드 위치 옮기기를 하자
	//지금은 카드 매니저에서 렌더 안 하니 괜찮다

	bool isControlable;
	float scrollSpeed = 100.0f;

	float xLength;

	Rect* leftTab;
	Vector2 leftPos;
	
	Rect* rightTab;
	Vector2 rightPos;
};

