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
	const float scrollSpeed = 200.0f;

	float xLength;

	ImageRect* leftTab;
	Vector2 leftPos;
	
	ImageRect* rightTab;
	Vector2 rightPos;
};

