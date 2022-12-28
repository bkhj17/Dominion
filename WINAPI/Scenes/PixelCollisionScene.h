#pragma once
class PixelCollisionScene : public Scene {
public:
	PixelCollisionScene();
	~PixelCollisionScene();
	// Scene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
private:
	ImageRect* land;
	ImageRect* background;

	Megaman* megaman;
};