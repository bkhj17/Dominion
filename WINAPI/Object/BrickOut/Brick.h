#pragma once
class Brick : public Rect
{
public:
	Brick(Vector2 pos, Vector2 size, UINT hp);
	~Brick();

	void Render(HDC hdc);

	Direction Collision(Circle* ball);

	void SetBrushes(HBRUSH* hBrushes) { this->hBrushes = hBrushes; }
private:
	UINT hp;

	HBRUSH *hBrushes= nullptr;
};

