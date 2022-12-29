#include "framework.h"

Bomb::Bomb()
	: ImageRect(L"Textures/Bomb/Bomb.bmp", 4, 1)
{
	isActive = false;
}

void Bomb::Update()
{
	if (!isActive)
		return;

	Fall();
	Timer();
}

void Bomb::Render(HDC hdc)
{
	ImageRect::CamRender(hdc, { (int)(time) ,0 });
}

void Bomb::Plant(const Vector2& pos)
{
	isActive = true;
	this->pos = pos;
	velocity = 0.0f;
	time = 0.0f;
}

void Bomb::Fall()
{
	velocity -= GRAVITY * DELTA;
	pos.y -= velocity * DELTA;

	float height = landTexture->GetPixelHeight(pos);
	if (Bottom() > height)
	{
		velocity = 0.0f;
		pos.y = height - Half().y;
	}
}

void Bomb::Timer()
{
	time += DELTA;
	if (time >= EXP_TIME) {
		isActive = false;

		//EffectManager::Get()->Play("Exp", bullet->pos);
		SelectObject(landTexture->GetMemDC(), hBrush);
		SelectObject(landTexture->GetMemDC(), hPen);

		int left = pos.x - EXP_RANGE;
		int right = pos.x + EXP_RANGE;
		int top = pos.y - EXP_RANGE;
		int bottom = pos.y + EXP_RANGE;


		Ellipse(landTexture->GetMemDC(), left, top, right, bottom);

		//나중에 매니저 생기면 충돌처리
		Observer::Get()->ExcuteParamEvents("SivaDamage", this);
	}
}
