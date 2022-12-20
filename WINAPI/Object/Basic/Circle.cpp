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

bool Circle::IsCollision(const Vector2& point) const
{
	float distance = (point-pos).Length();

	return distance < radius;
}

bool Circle::IsCollision(const Circle* circle) const
{
	float x = pos.x - circle->pos.x;
	float y = pos.y - circle->pos.y;

	float distance = sqrt(x * x + y * y);

	return distance < radius+circle->radius;
}

bool Circle::IsCollision(Rect* rect) const
{
	Rect bound(pos, Vector2(radius, radius));

	if (rect->IsCollision(&bound)) {
		if (pos.x < rect->Left() && pos.y < rect->Top()) {
			return IsCollision(Vector2(rect->Left(), rect->Top()));
		}
		if (pos.x > rect->Right() && pos.y < rect->Top()) {
			return IsCollision(Vector2(rect->Right(), rect->Top()));
		}
		if (pos.x < rect->Left() && pos.y > rect->Bottom()) {
			return IsCollision(Vector2(rect->Left(), rect->Bottom()));
		}
		if (pos.x > rect->Right() && pos.y > rect->Bottom()) {
			return IsCollision(Vector2(rect->Right(), rect->Bottom()));
		}

		return true;
	}
	return false;
}
