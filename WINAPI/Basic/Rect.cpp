#include "framework.h"
#include "Rect.h"

Rect::Rect(int width, int height)
	: pos({ 0,0 }), size({ width, height })
{
}

void Rect::Render(HDC hdc)
{
	int left = pos.x - size.x / 2;
	int right = pos.x + size.x / 2;
	int top = pos.y - size.y / 2;
	int bottom = pos.y + size.y / 2;

	Rectangle(hdc, left, top, right, bottom);
}
