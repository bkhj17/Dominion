#include "framework.h"
#include "Fighter.h"

Fighter::Fighter()
	: BattleCharacter(L"Textures/Fight/Fighter.bmp", 3, 13)
{
	CreateAnimations();
	SetAction(IDLE);
	animations[curType]->Play();

	size = { 70, 120 };
	imageOffset = { 35, 0 };

	EffectManager::Get()->Add("Punch", 3, L"Textures/Fight/Effect.bmp", 4, 4);
	EffectManager::Get()->Add("Kick", 3, L"Textures/Fight/Effect.bmp", 8, 1);
}

Fighter::~Fighter()
{
}

void Fighter::Update()
{
	Control();
	__super::Update();
	Attack();
}

void Fighter::Render(HDC hdc)
{
	__super::Render(hdc);
}

void Fighter::Control()
{

	if (isAttack)
		return;

	bool isMove = false;

	if (KEY_DOWN(VK_RIGHT) || KEY_DOWN(VK_LEFT)) {
		isMoveTouch = true;
		if (isMoveTouch) {
			isRun = true;
			isMove = true;
		}
		else
			isMoveTouch = true;
	}
	
	CheckMoveTouchTime();


	if (KEY_PRESS(VK_RIGHT)) {
		isMove = true;
		velocity.x = isRun ? RUN_SPEED : WALK_SPEED;
	}
	if (KEY_PRESS(VK_LEFT)) {
		isMove = true;
		velocity.x = isRun ? -RUN_SPEED : -WALK_SPEED ;
	}
	if (KEY_PRESS(VK_UP)) {
		isMove = true;
		velocity.y = isRun ? -RUN_SPEED : -WALK_SPEED;
	}
	if (KEY_PRESS(VK_DOWN)) {
		isMove = true;
		velocity.y = isRun ? RUN_SPEED : WALK_SPEED;
	}


	if (KEY_UP(VK_RIGHT) || KEY_UP(VK_LEFT)) {
		velocity.x = 0.0f;
	}

	if (KEY_UP(VK_RIGHT) || KEY_UP(VK_LEFT)) {
		velocity.y = 0.0f;
	}

	if (!isMove) {
		velocity = {0.0f,0.0f};
		isRun = false;
	}
}

void Fighter::Attack()
{
	if (isAttack)
		return;

	if (KEY_DOWN('Z')) {
		//ÆÝÄ¡
		SetAction(PUNCH);
		isAttack = true;

		attackCollider->isActive = true;
		attackCollider->pos = pos + Vector2(50, 0);
		attackCollider->size = { 100.0f,70.0f };

		EffectManager::Get()->Play("Punch", attackCollider->pos);
	}
	if (KEY_DOWN('X')) {
		//Å±
		SetAction(KICK);
		isAttack = true;

		attackCollider->isActive = true;
		attackCollider->pos = pos + Vector2(50, 10);
		attackCollider->size = { 80.0f,40.0f };

		EffectManager::Get()->Play("Kick", attackCollider->pos);
	}
}

void Fighter::SetAnimation()
{
	if (isAttack)
		return;

	if (velocity.Length() > 0.0f) {
		if (isRun)
			SetAction(RUN);
		else
			SetAction(WALK);
	}
	else
		SetAction(IDLE);
}

void Fighter::CreateAnimations()
{
	animations[ActionType::IDLE] = new Animation(texture->GetFrame());
	animations[ActionType::IDLE]->SetPart(23, 26);
	animations[ActionType::WALK] = new Animation(texture->GetFrame());
	animations[ActionType::WALK]->SetPart(27, 34);
	animations[ActionType::RUN] = new Animation(texture->GetFrame());
	animations[ActionType::RUN]->SetPart(7, 14);
	animations[ActionType::PUNCH] = new Animation(texture->GetFrame());
	animations[ActionType::PUNCH]->SetPart(0, 6, false);
	animations[ActionType::PUNCH]->SetEndEvent(bind(&Fighter::EndAttack, this));
	animations[ActionType::KICK] = new Animation(texture->GetFrame());
	animations[ActionType::KICK]->SetPart(35, 38, false);
	animations[ActionType::KICK]->SetEndEvent(bind(&Fighter::EndAttack, this));
}

void Fighter::CheckMoveTouchTime()
{
	if (!isMoveTouch)
		return;

	touchTime += DELTA;
	if (touchTime >= RUN_TOUCH_DELAY) {
		isMoveTouch = false;
		touchTime = 0.0f;
	}
}
