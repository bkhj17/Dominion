#pragma once

class TestScene : public Scene {
public:
	TestScene();
	~TestScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	class TestRect* rect;

	bool requestControl = true;
};