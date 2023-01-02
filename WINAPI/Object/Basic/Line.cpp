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
	if (rect->IsPointCollision(pos) || rect->IsPointCollision(end))
		return true;

	Vector2 leftTop(rect->Left(), rect->Top());
	Vector2 leftBottom(rect->Left(), rect->Bottom());
	Vector2 rightTop(rect->Right(), rect->Top());
	Vector2 rightBottom(rect->Right(), rect->Bottom());

	Line lines[4]; 
	lines[0] = Line(leftTop, leftBottom);
	lines[1] = Line(leftTop, rightTop);
	lines[2] = Line(rightTop, rightBottom);
	lines[3] = Line(rightBottom, leftBottom);

	for (auto line : lines) {
		if (line.IsLineCollision(this))
			return true;
	}

	return false;
}

bool Line::IsCircleCollision(const Circle* circle) const
{
	Vector2 out;
	return IsCircleCollision(circle, out);
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
	Vector2 A = circle->pos - pos;
	Vector2 B = end - pos;
	float dot = Dot(A, B) / B.Length();

	dot = Clamp(0.0f, B.Length(), dot);

	closestPoint = pos + B.GetNormalized()*dot;

	return circle->IsPointCollision(closestPoint);
}

bool Line::IsBetweenLine(const Line* line) const
{
	Vector2 e1 = line->pos - pos;
	Vector2 e2 = line->end - pos;
	Vector2 e3 = end - pos;

	return Cross(e1, e3) * Cross(e2, e3) < 0;
}