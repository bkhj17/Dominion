#include "framework.h"
#include "Siva.h"

Siva::Siva()
	: PixelCharacter(L"Textures/SivaRun.bmp", 8, 2)
{
	CreateAnimations();
	animations[IDLE_LEFT]->Play();

	Observer::Get()->AddParamEvent("SivaDamage", bind(&Siva::Damage, this, placeholders::_1));
}

Siva::~Siva()
{
}

void Siva::Update()
{
	ActionState();
	PixelCharacter::Update();
}

void Siva::Render(HDC hdc)
{
	ImageRect::CamRender(hdc, (int)alphaValue, animations[curType]->GetFrame());
}

void Siva::Damage(void* bomb)
{
	state = HIT;

	GameObject* object = (GameObject*)bomb;

	SetAction(velocity.x > 0 ? IDLE_RIGHT : IDLE_LEFT);
}

void Siva::Idle()
{
	idleTime += DELTA;
	if (idleTime > IDLE_RATE)
		StartPatrol();
}

void Siva::Patrol()
{
	if (CanMove())
	{
		Move();
	}
	else {
		velocity.x = -velocity.x;
	}
	patroltime += DELTA;
	if (patroltime >= PATROL_TIME) {
		state = IDLE;
		idleTime = 0.0f;
		if (velocity.x > 0.0f)
			SetAction(IDLE_RIGHT);
		else
			SetAction(IDLE_LEFT);
	}
}

void Siva::Hit()
{
	hitTime += DELTA;

	if (isIncreaseAlpha) {
		alphaValue += BLINK_SPEED * DELTA;
		if (alphaValue > 255.0f)
			isIncreaseAlpha = false;
	}
	else {
		alphaValue -= BLINK_SPEED * DELTA;
		if (alphaValue < 0.0f)
			isIncreaseAlpha = false;
	}

	if (hitTime >= HIT_DELAY) {
		hitTime = 0.0f;
		alphaValue = 255;
		state = IDLE;
	}
}

void Siva::Move()
{
	pos.x += velocity.x * DELTA;

	if (velocity.x > 0.0f)
		SetAction(RUN_RIGHT);
	else if (velocity.x < 0.0f)
		SetAction(RUN_LEFT);

}

void Siva::StartPatrol()
{
	patroltime = 0.0f;
	state = PATROL;
	velocity.x = rand() % 2 ? -speed : speed;
}

void Siva::ActionState()
{
	switch (state)
	{
	case Siva::IDLE:
		Idle();
		break;
	case Siva::PATROL:
		Patrol();
		break;
	case Siva::HIT:
		Hit();
		break;
	default:
		break;
	}

}

void Siva::CreateAnimations()
{

	animations[IDLE_LEFT] = new Animation(texture->GetFrame(), 0.5f);
	animations[IDLE_LEFT]->SetPart(14, 15);

	animations[IDLE_RIGHT] = new Animation(texture->GetFrame(), 0.5f);
	animations[IDLE_RIGHT]->SetPart(0, 1);

	animations[RUN_LEFT] = new Animation(texture->GetFrame());
	animations[RUN_LEFT]->SetPart(8, 13);

	animations[RUN_RIGHT] = new Animation(texture->GetFrame());
	animations[RUN_RIGHT]->SetPart(2, 7);
}
