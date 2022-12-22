#include "framework.h"
#include "Line.h"

Line::Line(Vector2 start, Vector2 end)
	: GameObject(start), end(end)
{
}

void Line::Render(HDC hdc)
{
	MoveToEx(hdc, pos.x, pos.y, nullptr);
	LineTo(hdc, end.x, end.y);
}

bool Line::IsPointCollision(const Vector2& point) const
{
	return false;
}

bool Line::IsRectCollision(const Rect* rect) const
{
	return false;
}

bool Line::IsCircleCollision(const Circle* circle) const
{
	return false;
}
