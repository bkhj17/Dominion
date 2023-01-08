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
	//��� �������� ������ ī���
	//ī�� ��ġ �ű�⸦ ����
	//������ ī�� �Ŵ������� ���� �� �ϴ� ������

	bool isControlable;
	float scrollSpeed = 100.0f;

	float xLength;

	Rect* leftTab;
	Vector2 leftPos;
	
	Rect* rightTab;
	Vector2 rightPos;
};

