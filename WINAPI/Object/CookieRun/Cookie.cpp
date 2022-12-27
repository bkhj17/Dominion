#include "Framework.h"

Cookie::Cookie()
	: Character(L"Textures/CookieRun.bmp", 11, 6)
{
	imageOffset = { 0.0f, -110.0f };

	CreateAnimations();
	animations[(int)curType]->Play();
}

Cookie::~Cookie()
{
}

void Cookie::Update()
{
	Jump();
	Crash();

	__super::Update();
}

void Cookie::Render(HDC hdc)
{
	Character::Render(hdc, (int)alphaValue);
}

void Cookie::Jump()
{
	if (jumpCount < 2 && KEY_DOWN(VK_UP) && StageManager::Get()->IsPlay()) {
		velocity = JUMP_POWER;
		if (++jumpCount == 2)
			SetAction((int)ActionType::DOUBLE_JUMP);
		else
			SetAction((int)ActionType::JUMP);
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
		SetAction((int)ActionType::JUMP_DOWN);
	}
	else {
		SetAction((int)ActionType::RUN);
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

	SetAction((int)ActionType::CRASH);
	isGhost = true;
	ghostTime = 0.0f;
	StageManager::Get()->Stop();
}

void Cookie::EndCrash()
{
	SetAction((int)ActionType::RUN);
	StageManager::Get()->Play();
}


void Cookie::CreateAnimations()
{
	animations[(int)ActionType::IDLE] = new Animation(texture->GetFrame());
	animations[(int)ActionType::IDLE]->SetPart(22, 31);

	animations[(int)ActionType::RUN] = new Animation(texture->GetFrame());
	animations[(int)ActionType::RUN]->SetPart(11, 18);

	animations[(int)ActionType::JUMP] = new Animation(texture->GetFrame());
	animations[(int)ActionType::JUMP]->SetPart(33, 37, false);

	animations[(int)ActionType::DOUBLE_JUMP] = new Animation(texture->GetFrame());
	animations[(int)ActionType::DOUBLE_JUMP]->SetPart(0, 5, false);

	animations[(int)ActionType::JUMP_DOWN] = new Animation(texture->GetFrame());
	animations[(int)ActionType::JUMP_DOWN]->SetPart(5, 5, false);

	animations[(int)ActionType::CRASH] = new Animation(texture->GetFrame());
	animations[(int)ActionType::CRASH]->SetPart(44, 49, false);
	animations[(int)ActionType::CRASH]->SetEndEvent(bind(&Cookie::EndCrash, this));
}
