#pragma once
class ShootingScene : public Scene {
public:
	ShootingScene();
	~ShootingScene();
	// Scene을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	Plane* plane = nullptr;
};