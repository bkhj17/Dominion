#include "framework.h"

Megaman::Megaman()
	: ImageRect(L"Textures/CookieRun.bmp", 11, 6)
{
	CreateAnimations();
	animations[curType]->Play();

	bodyOffset = { 0,120 };
	bodyRect = new Rect(pos+bodyOffset, {95, 133});
}

Megaman::~Megaman()
{
	for (pair<ActionType, Animation*> animation : animations) {
		delete animation.second;
	}

	delete bodyRect;
}

void Megaman::Update()
{
	Move();
	Jump();

	bodyRect->pos = pos + bodyOffset;

	SetAnimation();
	animations[curType]->Update();
}

void Megaman::Render(HDC hdc)
{
	bodyRect->LineRender(hdc);
	ImageRect::Render(hdc, true, animations[curType]->GetFrame());
}

void Megaman::Move()
{
	bool isMove = false;

	if (KEY_PRESS(VK_RIGHT)) {
		velocity.x = speed * DELTA;
		isMove = true;
	}
	if (KEY_PRESS(VK_LEFT)) {
		velocity.x = -speed * DELTA;
		isMove = true;
	}

	if (!isMove) {
		velocity.x = 0.0f;
	}

}

void Megaman::Jump()
{

	if (jumpCount < 2 && KEY_DOWN(VK_UP)) {
		velocity.y = JUMP_POWER;
		jumpCount++;
	}

	velocity.y -= GRAVITY * DELTA;
	pos.y -= velocity.y * DELTA;
	/*
	if (Bottom() > WIN_HEIGHT) {
		velocity.y = 0.0f;
		pos.y = WIN_HEIGHT - Half().y;

		jumpCount = 0;
	}
	*/
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
