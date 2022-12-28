#include "framework.h"

Megaman::Megaman()
	: ImageRect(L"Textures/CookieRun.bmp", 11, 6)
{
	CreateAnimations();
	animations[curType]->Play();

	BulletManager::Get();

	bodyOffset = { 0,120 };
	bodyRect = new Rect(pos+bodyOffset, {95, 133});

	hBrush = CreateSolidBrush(RGB(0, 178, 255));
	hPen = CreatePen(PS_SOLID, 10, RGB(0, 178, 255));
}

Megaman::~Megaman()
{
	for (pair<ActionType, Animation*> animation : animations) {
		delete animation.second;
	}

	delete bodyRect;
	DeleteObject(hBrush);
	DeleteObject(hPen);

}

void Megaman::Update()
{
	Fire();
	Move();
	Jump();
	SetAnimation();

	bodyRect->pos = pos + bodyOffset;

	animations[curType]->Update();

	BulletManager::Get()->Update();
}

void Megaman::Render(HDC hdc)
{
	//bodyRect->LineRender(hdc);
	ImageRect::CamRender(hdc, animations[curType]->GetFrame());

	BulletManager::Get()->Render(hdc);
}

void Megaman::Fire()
{
	if (KEY_DOWN(VK_SPACE)) {
		BulletManager::Get()->Fire(pos, Vector2(1, -1));
	}

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
}

void Megaman::Move()
{
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

	pos.x += velocity.x * DELTA;
}

void Megaman::Jump()
{
	if (jumpCount < 2 && KEY_DOWN(VK_UP)) {
		velocity.y = JUMP_POWER;
		jumpCount++;

		if (jumpCount >= 2)
		{
			SetAction(ActionType::JUMP_DOWN);
		}
	}

	velocity.y -= GRAVITY * DELTA;
	pos.y -= velocity.y * DELTA;
	
	Vector2 bottomPos = pos;
	bottomPos.y = Bottom() - 20.0f;
	float height = landTexture->GetPixelHeight(bottomPos);

	if (Bottom() > height) {
		if (velocity.y < 0.0f) {
			velocity.y = 0.0f;
			jumpCount = 0;
			pos.y = height - Half().y;
		}
	}
}

void Megaman::SetAnimation()
{
	if (velocity.y < -1.0f) {
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

void Megaman::SetAction(ActionType type)
{
	if (curType == type)
		return;

	curType = type;
	animations[curType]->Play();
}

void Megaman::CreateAnimations()
{
	animations[ActionType::IDLE] = new Animation(texture->GetFrame());
	animations[ActionType::IDLE]->SetPart(35, 37);

	animations[ActionType::RUN] = new Animation(texture->GetFrame());
	animations[ActionType::RUN]->SetPart(11, 18);

	animations[ActionType::JUMP_UP] = new Animation(texture->GetFrame());
	animations[ActionType::JUMP_UP]->SetPart(0, 0, false);

	animations[ActionType::JUMP_DOWN] = new Animation(texture->GetFrame());
	animations[ActionType::JUMP_DOWN]->SetPart(1, 1, false);
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
