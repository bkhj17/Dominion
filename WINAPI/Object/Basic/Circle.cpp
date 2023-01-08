#include "Framework.h"

Circle::Circle(Vector2 pos, float radius)
	: GameObject(pos), radius(radius)
{
	isActive = true;
}

void Circle::Render(HDC hdc)
{
	if (!isActive)
		return;

	Ellipse(hdc, 
		(int)(pos.x - radius),
		(int)(pos.y - radius),
		(int)(pos.x + radius), 
		(int)(pos.y + radius));
}

bool Circle::IsPointCollision(const Vector2& point) const
{
	float distance = (point-pos).Length();

	return distance < radius;
}

bool Circle::IsCircleCollision(const Circle* circle) const
{
	float x = pos.x - circle->pos.x;
	float y = pos.y - circle->pos.y;

	float distance = sqrt(x * x + y * y);

	return distance < radius+circle->radius;
}

bool Circle::IsRectCollision(const Rect* cardRect) const
{
	Vector2 edges[4] = {
		{cardRect->Left(), cardRect->Top()},
		{cardRect->Right(), cardRect->Top()},
		{cardRect->Left(), cardRect->Bottom()},
		{cardRect->Right(), cardRect->Bottom()}
	};


	if (cardRect->Left() < pos.x+radius 
		&& cardRect->Right() > pos.x-radius
		&& cardRect->Top() < pos.y+radius
		&& cardRect->Bottom() > pos.y-radius) {

		if (pos.x < cardRect->Left() && pos.y < cardRect->Top()) {
			return IsPointCollision(edges[0]);
		}
		if (pos.x > cardRect->Right() && pos.y < cardRect->Top()) {
			return IsPointCollision(edges[1]);
		}
		if (pos.x < cardRect->Left() && pos.y > cardRect->Bottom()) {
			return IsPointCollision(edges[2]);
		}
		if (pos.x > cardRect->Right() && pos.y > cardRect->Bottom()) {
			return IsPointCollision(edges[3]);
		}

		return true;
	}
	return false;
}

bool Circle::IsLineCollision(const Line* line) const
{
	return false;
}
