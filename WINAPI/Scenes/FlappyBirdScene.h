#pragma once
class FlappyBirdScene : public Scene {
public:
	FlappyBirdScene();
	~FlappyBirdScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
private:
	LandScape* landScape;
	FlappyBird* flappyBird;
};