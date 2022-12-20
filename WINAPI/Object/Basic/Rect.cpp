#include "framework.h"
#include "Rect.h"


Rect::Rect(Vector2 pos, Vector2 size)
	: GameObject(pos), size(size)
{
}

Rect::Rect(float left, float top, float right, float bottom)
	: GameObject(Vector2((left+right)*0.5f, (top+bottom)*0.5f))
	, size(abs(right - left), abs(bottom - top))
{
}

void Rect::Render(HDC hdc)
{
	if (!isActive)
		return;

	int left = (int)(pos.x - size.x *0.5f);
	int right = (int)(pos.x + size.x * 0.5f);
	int top = (int)(pos.y - size.y * 0.5f);
	int bottom = (int)(pos.y + size.y * 0.5f);

	Rectangle(hdc, left, top, right, bottom);
}

bool Rect::IsCollision(const Vector2& point) const
{
	return Active()
		&& point.x > Left() && point.x < Right() 
		&& point.y > Top() && point.y < Bottom();
}

bool Rect::IsCollision(const Circle* point) const
{
	return false;
}

bool Rect::IsCollision(const Rect* rect) const
{
	if(!isActive || !rect->isActive)
		return false;

	if (rect->Left() < Right() && rect->Right() > Left()) {
		if (rect->Top() < Bottom() && rect->Bottom() > Top())
			return true;
	}
	return false;
}
