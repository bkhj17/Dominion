#include "Framework.h"

Cookie::Cookie()
{
	texture = Texture::Add(L"Textures/CookieRun.bmp", 11, 6);
	imageRect = new ImageRect(texture);
	size = { 120, 150 };
	ImageOffset = { 0.0f, -110.0f };

	CreateAnimations();
	animations[curType]->Play();
}


Cookie::~Cookie()
{
	delete imageRect;

	for (pair<ActionType, Animation*> animation : animations) {
		delete animation.second;
	}
}

void Cookie::Update()
{
	Jump();
	SetAnimation();
	Crash();
	imageRect->pos = pos+ImageOffset;

	animations[curType]->Update();
}

void Cookie::Render(HDC hdc)
{
	Rect::LineRender(hdc);

	imageRect->Render(hdc, (int)alphaValue, animations[curType]->GetFrame());
}

void Cookie::Jump()
{
	if (jumpCount < 2 && KEY_DOWN(VK_UP) && StageManager::Get()->IsPlay()) {
		velocity = JUMP_POWER;
		if (++jumpCount == 2)
			SetAction(ActionType::DOUBLE_JUMP);
		else
			SetAction(ActionType::JUMP);
	}

	velocity -= GRAVITY * DELTA;
	pos.y -= velocity * DELTA;

	Vector2 bottomPos = { pos.x, Bottom() - BOTTOM_CHECK };
	float groundHeight = StageManager::Get()->GetGroundHeight(bottomPos);

	if (Bottom() > groundHeight) {
		if (velocity < 0.0f) {
			velocity = 0.0f;
			jumpCount = 0;
			pos.y = groundHeight - Half().y;
		}
	}
}

void Cookie::SetAnimation()
{
	if (curType == ActionType::CRASH)
		return;
	if (velocity > 0.0f) return;

	if (velocity < 0.0f) {
		SetAction(ActionType::JUMP_DOWN);
	}
	else {
		SetAction(ActionType::RUN);
	}
}

void Cookie::Crash()
{
	if (isGhost) {
		ghostTime += DELTA;
		
		if (isIncreaseAlpha) {
			alphaValue += ALPHA_SPEED * DELTA;
			if (alphaValue > 255.0f)
				isIncreaseAlpha = false;
		}
		else {
			alphaValue -= ALPHA_SPEED * DELTA;
			if (alphaValue < 0.0f)
				isIncreaseAlpha = true;
		}

		if (ghostTime >= GHOST_TIME) {
			alphaValue = 255.0f;
			isGhost = false;
		}
		return;
	}

	if (!StageManager::Get()->CollisionObstacle(this))
		return;

	SetAction(ActionType::CRASH);
	isGhost = true;
	ghostTime = 0.0f;
	StageManager::Get()->Stop();
}

void Cookie::EndCrash()
{
	SetAction(ActionType::RUN);
	StageManager::Get()->Play();
}

void Cookie::SetAction(ActionType type)
{
	if (curType == type)
		return;

	curType = type;
	animations[curType]->Play();
}


void Cookie::CreateAnimations()
{
	animations[ActionType::IDLE] = new Animation(texture->GetFrame());
	animations[ActionType::IDLE]->SetPart(22, 31);

	animations[ActionType::RUN] = new Animation(texture->GetFrame());
	animations[ActionType::RUN]->SetPart(11, 18);

	animations[ActionType::JUMP] = new Animation(texture->GetFrame());
	animations[ActionType::JUMP]->SetPart(33, 37, false);

	animations[ActionType::DOUBLE_JUMP] = new Animation(texture->GetFrame());
	animations[ActionType::DOUBLE_JUMP]->SetPart(0, 5, false);

	animations[ActionType::JUMP_DOWN] = new Animation(texture->GetFrame());
	animations[ActionType::JUMP_DOWN]->SetPart(5, 5, false);

	animations[ActionType::CRASH] = new Animation(texture->GetFrame());
	animations[ActionType::CRASH]->SetPart(44, 49, false);
	animations[ActionType::CRASH]->SetEndEvent(bind(&Cookie::EndCrash, this));
}
