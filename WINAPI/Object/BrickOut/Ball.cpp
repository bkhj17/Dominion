#include "Framework.h"

Ball::Ball()
{
	radius = BALL_SIZE;
	hBrush = CreateSolidBrush(BLACK);

	aimingDirection = new Line();
}

Ball::~Ball()
{
	DeleteObject(hBrush);
}

void Ball::Update()
{
	if (KEY_DOWN(VK_SPACE))
		isPlay = true;

	Move();
	CollisionBorder();
	CollisionBar();
	CollisionBrick();
	AimControl();
}

void Ball::Render(HDC hdc)
{
	SelectObject(hdc, hBrush);
	__super::Render(hdc);
	
	if(!isPlay)
		aimingDirection->Render(hdc);
}

void Ball::Move()
{
	if (!isPlay) {
		pos.x = barRect->pos.x;
		pos.y = barRect->Top() - radius;
	}
	else {
		pos += direction * speed *  DELTA;
	}
}

void Ball::AimControl()
{
	if (isPlay)
		return;

	if (KEY_PRESS(VK_UP))
		aimingAngle += DELTA;

	if (KEY_PRESS(VK_DOWN))
		aimingAngle -= DELTA;

	aimingDirection->pos = pos;
	aimingDirection->end.x = pos.x + cosf(aimingAngle) * aimLength;
	aimingDirection->end.y = pos.y + -sinf(aimingAngle) * aimLength;

	direction = (aimingDirection->end - pos).GetNormalized();
}

void Ball::CollisionBorder()
{
	if (pos.x > WIN_WIDTH - radius) {
		pos.x = WIN_WIDTH - radius;
		direction.x *= -1.f;
	} else if (pos.x < radius) {
		pos.x = radius;
		direction.x *= -1.f;
	}

	if (pos.y < radius) {
		direction.y *= -1.f;
		pos.y = radius;
	}

	if (pos.y > WIN_HEIGHT + radius) {
		isPlay = false;
		barRect->pos.x = CENTER_X;
		aimingAngle = PI * 0.5f;
	}
}

void Ball::CollisionBar()
{
	if (!barRect->IsCircleCollision(this))
		return;
	
	direction = (pos - barRect->pos).GetNormalized();
}

void Ball::CollisionBrick()
{
	Direction dir = BrickManager::Get()->Collision(this);
	switch (dir)
	{
	case GameMath::Direction::UP:
	case GameMath::Direction::DOWN:
		direction.y *= -1.0f;
		break;
	case GameMath::Direction::LEFT:
	case GameMath::Direction::RIGHT:
		direction.x *= -1.0f;
		break;
	case GameMath::Direction::NONE:
		break;
	}

}
