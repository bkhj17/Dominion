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
	class InfoBox* infoBox;

	deque<class DominionPlayer*> winner;
};

