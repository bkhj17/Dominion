#pragma once
class DrawShape
{
	static BOOL Draw(HDC hdc, int x, int y, int width, int height, HPEN hPen, BOOL (func)(HDC, int, int, int, int));
public:
	static BOOL DrawRect(HDC hdc, int x, int y, int width, int height, HPEN hPen = nullptr);
	static BOOL DrawEllipse(HDC hdc, int x, int y, int width, int height, HPEN hPen = nullptr);

};

