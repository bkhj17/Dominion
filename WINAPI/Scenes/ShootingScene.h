#pragma once
class ShootingScene : public Scene {
public:
	ShootingScene();
	~ShootingScene();
	// Scene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	Plane* plane = nullptr;
};