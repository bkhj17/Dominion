#pragma once
class Rect
{
public:
	Rect(int width = 100, int height = 100);

	void Render(HDC hdc);

	POINT& Pos() { return pos; }

	const POINT& GetSize() { return size; }
private:
	POINT pos;
	POINT size;
};

