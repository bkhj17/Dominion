#pragma once
class CookieRunScene : public Scene {
public:
	CookieRunScene();
	~CookieRunScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
private:
	LandScape* landscape;
	Cookie* cookie;
};