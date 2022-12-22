#pragma once
class ControlBar : public Rect {
private:
	const float BAR_WIDTH = 120.0f;
	const float BAR_HEIGHT = 20.0f;
public:
	ControlBar();
	~ControlBar();

	void Update() override;
	void Render(HDC hdc) override;
private:
	void Control();

private:
	float speed = 300.0f;

	HBRUSH hBrush = nullptr;




};