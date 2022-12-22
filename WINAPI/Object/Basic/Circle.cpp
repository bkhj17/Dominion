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

bool Circle::IsRectCollision(const Rect* rect) const
{


	Vector2 edges[4] = {
		{rect->Left(), rect->Top()},
		{rect->Right(), rect->Top()},
		{rect->Left(), rect->Bottom()},
		{rect->Right(), rect->Bottom()}
	};


	if (rect->Left() < pos.x+radius 
		&& rect->Right() > pos.x-radius
		&& rect->Top() < pos.y+radius
		&& rect->Bottom() > pos.y-radius) {


		if (pos.x < rect->Left() && pos.y < rect->Top()) {
			return IsPointCollision(edges[0]);
		}
		if (pos.x > rect->Right() && pos.y < rect->Top()) {
			return IsPointCollision(edges[1]);
		}
		if (pos.x < rect->Left() && pos.y > rect->Bottom()) {
			return IsPointCollision(edges[2]);
		}
		if (pos.x > rect->Right() && pos.y > rect->Bottom()) {
			return IsPointCollision(edges[3]);
		}

		return true;
	}
	return false;
}
