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

void Rect::LineRender(HDC hdc)
{

	if (!isActive)
		return;

	MoveToEx(hdc, (int)Left(), (int)Top(), nullptr);
	LineTo(hdc, (int)Left(), (int)Bottom());
	LineTo(hdc, (int)Right(), (int)Bottom());
	LineTo(hdc, (int)Right(), (int)Top());
	LineTo(hdc, (int)Left(), (int)Top());
}

bool Rect::IsPointCollision(const Vector2& point) const
{
	return isActive
		&& point.x > Left() && point.x < Right() 
		&& point.y > Top() && point.y < Bottom();
}

bool Rect::IsCircleCollision(const Circle* circle) const
{
	return circle->IsCollision(this);
}

bool Rect::IsRectCollision(const Rect* rect) const
{
	if(!isActive || !rect->isActive)
		return false;

	if (rect->Left() < Right() && rect->Right() > Left()) {
		if (rect->Top() < Bottom() && rect->Bottom() > Top())
			return true;
	}
	return false;
}

bool Rect::IsRectCollision(IN const Rect* rect, OUT Vector2* overlapSize) const
{
	if (!isActive || !rect->isActive)
		return false;

	float left = max(rect->Left(), Left());
	float right = min(rect->Right(), Right());
	float top = max(rect->Top(), Top());
	float bottom = min(rect->Bottom(), Bottom());

	overlapSize->x = right - left;
	overlapSize->y = bottom - top;
	if (overlapSize->x > 0 && overlapSize->y > 0)
		return true;
	
	return false;
}
