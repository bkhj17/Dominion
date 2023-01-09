#pragma once

class DominionScene : public Scene
{
public:
	DominionScene();
	~DominionScene();

	virtual void Start() override;
	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
private:
	void RenderGameEnd(HDC hdc);

private:
	class InfoBox* infoBox;

	deque<class DominionPlayer*> winner;

	ImageRect* background;

	HFONT font;
};

