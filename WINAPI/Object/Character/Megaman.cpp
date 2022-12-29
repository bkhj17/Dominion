#include "framework.h"

Megaman::Megaman()
	: PixelCharacter(L"Textures/CookieRun.bmp", 11, 6)
{
	CreateAnimations();
	animations[curType]->Play();


	bodyOffset = { 0,120 };
	bodyRect = new Rect(pos+bodyOffset, {95, 133});

}

Megaman::~Megaman()
{
	delete bodyRect;
}

void Megaman::Update()
{
	Control();
	Move();
	__super::Update();
	SetAnimation();

	bodyRect->pos = pos + bodyOffset;

	animations[curType]->Update();
}

void Megaman::Fire()
{
	if (curType != ActionType::PLANT)
		BombManager::Get()->Plant({ pos.x, pos.y });
	/*
	GameObject* bullet = BulletManager::Get()->CollisionLand(landTexture);
	if (bullet) {
		EffectManager::Get()->Play("Exp", bullet->pos);
		SelectObject(landTexture->GetMemDC(), hBrush);
		SelectObject(landTexture->GetMemDC(), hPen);

		int left = bullet->pos.x - expRange;
		int right = bullet->pos.x + expRange;
		int top = bullet->pos.y - expRange;
		int bottom = bullet->pos.y + expRange;

		bullet->isActive = false;
		Ellipse(landTexture->GetMemDC(), left, top, right, bottom);
	}
	*/
}

void Megaman::Move()
{
	if (curType == (int)ActionType::PLANT)
		return;

	if (CanMove())
		pos.x += velocity.x * DELTA;
}

void Megaman::Control()
{
	if (KEY_DOWN(VK_SPACE)) {
		Fire();
	}

	bool isMove = false;

	if (KEY_PRESS(VK_RIGHT)) {
		velocity.x = speed;
		isMove = true;
	}
	if (KEY_PRESS(VK_LEFT)) {
		velocity.x = -speed;
		isMove = true;
	}

	if (!isMove) {
		velocity.x = 0.0f;
	}

	if (jumpCount < 2 && KEY_DOWN(VK_UP)) {
		velocity.y = JUMP_POWER;
		jumpCount++;

		if (jumpCount >= 2)
		{
			SetAction(ActionType::JUMP_DOWN);
		}
	}
}

void Megaman::SetAnimation()
{
	if (velocity.y < -3.0f) {
		SetAction(ActionType::JUMP_DOWN);
		return;
	}
	else if (velocity.y > 1.0f) {
		SetAction(ActionType::JUMP_UP);
		return;
	}

	if (abs(velocity.x) > 0.0f)
		SetAction(ActionType::RUN);
	else
		SetAction(ActionType::IDLE);
}


void Megaman::CreateAnimations()
{
	animations[IDLE] = new Animation(texture->GetFrame());
	animations[IDLE]->SetPart(35, 37);

	animations[RUN] = new Animation(texture->GetFrame());
	animations[RUN]->SetPart(11, 18);

	animations[JUMP_UP] = new Animation(texture->GetFrame());
	animations[JUMP_UP]->SetPart(0, 0, false);

	animations[JUMP_DOWN] = new Animation(texture->GetFrame());
	animations[JUMP_DOWN]->SetPart(1, 1, false);

	animations[PLANT] = new Animation(texture->GetFrame());
	animations[PLANT]->SetPart(1, 1, false);
	animations[PLANT]->SetEndEvent(bind(&Megaman::SetIdle, this));
}

bool Megaman::LandCollision(Rect* land)
{
	Vector2 overlapSize;
	if (land->IsRectCollision(bodyRect, &overlapSize)) {
		if (overlapSize.x > overlapSize.y) {
			//»óÇÏ
			if (land->pos.y > pos.y) {
				pos.y -= overlapSize.y;
				velocity.y = max(velocity.y, 0.0f);
				jumpCount = 0;
			}
			else {
				pos.y += overlapSize.y;
				velocity.y = min(velocity.y, 0.0f);
			}

			return true;
		}
		else {
			//ÁÂ¿ì
			if (land->pos.x > pos.x) {
				velocity.x = min(velocity.x, 0.0f);
			}
			else {
				velocity.x = max(velocity.x, 0.0f);
			}
		}
	}
	return false;
}
