#pragma once

class TestScene : public Scene {
public:
	TestScene();
	~TestScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Start() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	class TestRect* cardRect;

	bool requestControl = true;
};