#pragma once
class PixelCollisionScene : public Scene {
public:
	PixelCollisionScene();
	~PixelCollisionScene();
	// Scene을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
private:
	ImageRect* land;
	ImageRect* background;

	Megaman* megaman;
};