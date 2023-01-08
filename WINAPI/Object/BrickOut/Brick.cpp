#include "framework.h"
#include "Brick.h"

Brick::Brick(Vector2 pos, Vector2 size, UINT hp)
	: Rect(pos, size), hp(hp)
{
	
}

Brick::~Brick()
{
}

void Brick::Render(HDC hdc)
{
	if (hp == 0)
		return;

	SelectObject(hdc, hBrushes[hp - 1]);
	__super::Render(hdc);
}

Direction Brick::Collision(Circle* ball)
{
	if (hp == 0)
		return Direction::NONE;

	if (!IsCircleCollision(ball))
		return Direction::NONE;

	hp--;
	
	Vector2 size(1.0f, 1.0f);
	size *= ball->GetRadius() * 2;
	Rect cardRect(ball->pos, size);

	Vector2 overlapSize;
	IsRectCollision(&cardRect, &overlapSize);

	if (overlapSize.x > overlapSize.y) {
		//»óÇÏ
		if (pos.y > ball->pos.y) {
			ball->pos.y -= overlapSize.y;
			return Direction::UP;
		}
		else {
			ball->pos.y += overlapSize.y;
			return Direction::DOWN;
		}
	}
	else {
		//ÁÂ¿ì
		if (pos.x > ball->pos.x) {
			ball->pos.x -= overlapSize.x;
			return Direction::LEFT;
		}
		else {
			ball->pos.x += overlapSize.x;
			return Direction::RIGHT;
		}
	}

	return Direction();
}
