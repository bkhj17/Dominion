#pragma once
class Plane : public Circle 
{
	const float BODY_SIZE = 30.0f;
public:
	Plane();
	~Plane();

	void Update();
	void Render(HDC hdc);

private:
	void RenderBody(HDC hdc);
	void MoveControl();
	void FireControl();

	void Damage();
private:
	float speed = 200.0f;

	HBRUSH hBrush;
	HPEN hPen;
};