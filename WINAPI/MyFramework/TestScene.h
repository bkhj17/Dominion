#pragma once

class TestScene : public Scene {
public:
	TestScene();
	~TestScene();

	// Scene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	class TestRect* rect;

	bool requestControl = true;
};