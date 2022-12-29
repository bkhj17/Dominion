#pragma once
class DigGameScene : public Scene
{
public:
	DigGameScene();
	~DigGameScene();
	
	// Scene��(��) ���� ��ӵ�
	virtual void Start() override;
	virtual void End() override;

	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	ImageRect* land;
	ImageRect* background;

	Megaman* megaman = nullptr;
	Siva* siva = nullptr;
};