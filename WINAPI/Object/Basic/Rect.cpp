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

bool Rect::IsLineCollision(const Line* line) const
{
	return false;
}

bool Rect::IsRectCollision(const Rect* cardRect) const
{
	if(!isActive || !cardRect->isActive)
		return false;

	if (cardRect->Left() < Right() && cardRect->Right() > Left()) {
		if (cardRect->Top() < Bottom() && cardRect->Bottom() > Top())
			return true;
	}
	return false;
}

bool Rect::IsRectCollision(IN const Rect* cardRect, OUT Vector2* overlapSize) const
{
	if (!isActive || !cardRect->isActive)
		return false;

	float left = max(cardRect->Left(), Left());
	float right = min(cardRect->Right(), Right());
	float top = max(cardRect->Top(), Top());
	float bottom = min(cardRect->Bottom(), Bottom());

	overlapSize->x = right - left;
	overlapSize->y = bottom - top;
	if (overlapSize->x > 0 && overlapSize->y > 0)
		return true;
	
	return false;
}
