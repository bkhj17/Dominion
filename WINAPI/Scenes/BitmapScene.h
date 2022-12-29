#pragma once
class BitmapScene : public Scene {
public:
	BitmapScene();
	~BitmapScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Start() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
private:
	HDC memDC = nullptr;
	HBITMAP testBitmap = nullptr;
	HBITMAP sivaBitmap = nullptr;
};