#pragma once
class AnimationScene : public Scene {
public:
	AnimationScene();
	~AnimationScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
private:
	Megaman* megaman;

};