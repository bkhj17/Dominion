#pragma once
class FlappyBirdScene : public Scene {
public:
	FlappyBirdScene();
	~FlappyBirdScene();

	// Scene을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
private:
	LandScape* landScape;
	FlappyBird* flappyBird;
};