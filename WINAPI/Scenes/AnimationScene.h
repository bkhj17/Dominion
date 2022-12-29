#pragma once
class AnimationScene : public Scene {
public:
	AnimationScene();
	~AnimationScene();

	// Scene을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
private:
	Megaman* megaman;

};