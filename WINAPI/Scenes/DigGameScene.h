#pragma once
class DigGameScene : public Scene
{
public:
	DigGameScene();
	~DigGameScene();
	
	// Scene을(를) 통해 상속됨
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