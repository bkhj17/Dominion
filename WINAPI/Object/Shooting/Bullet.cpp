#include "framework.h"

Bullet::Bullet()
{
	radius = SIZE;
	isActive = false;
}

Bullet::~Bullet()
{
}

void Bullet::Update()
{
	if (!isActive)
		return;


	direction.y += DELTA;
	pos += direction * speed * DELTA;

	/*
	if (pos.y < radius)
		isActive = false;
	*/

	if (pos.y > 1000.0f)
		isActive = false;
}

void Bullet::Render(HDC hdc)
{
	
	Vector2 tmp = pos;
	pos -= Camera::Get()->GetPos();
	Circle::Render(hdc);
	pos = tmp;



	/*
	MoveToEx(hdc, (int)pos.x, (int)(pos.y - radius), nullptr);
	LineTo(hdc, (int)(pos.x- radius), (int)(pos.y+ radius));

	MoveToEx(hdc, (int)pos.x, (int)(pos.y - radius), nullptr);
	LineTo(hdc, (int)(pos.x + radius), (int)(pos.y + radius));
	*/
}

void Bullet::Fire(const Vector2& pos, Vector2 direction)
{
	isActive = true;
	this->pos = pos;
	this->direction = direction.GetNormalized();
}
