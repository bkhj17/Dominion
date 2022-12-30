#include "framework.h"
#include "Line.h"

Line::Line(Vector2 start, Vector2 end)
	: GameObject(start), end(end)
{
}

void Line::Render(HDC hdc)
{
	MoveToEx(hdc, (int)pos.x, (int)pos.y, nullptr);
	LineTo(hdc, (int)end.x, (int)end.y);
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

bool Line::IsCircleCollision(const Circle* circle, Vector2& closestPoint) const
{
	Vector2 e1 = end - pos;
	Vector2 e2 = circle->pos - pos;

	return false;
}

bool Line::IsLineCollision(const Line* line) const
{
	return IsBetweenLine(line) && line->IsBetweenLine(this);
}

bool Line::IsLineCollision(IN const Line* line, OUT Vector2& cross_point)
{
	return IsLineCollision(line);
}

bool Line::IsCircleCollision(const Circle* circle, OUT Vector2& closestPoint) const
{
	return false;
}

bool Line::IsBetweenLine(const Line* line) const
{
	Vector2 e1 = line->pos - pos;
	Vector2 e2 = line->end - pos;
	Vector2 e3 = end - pos;

	return Cross(e1, e3) * Cross(e2, e3) < 0;
}