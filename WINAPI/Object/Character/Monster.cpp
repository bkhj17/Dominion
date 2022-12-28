#include "framework.h"

Monster::Monster()
	: BattleCharacter(L"Textures/Fight/Monster.bmp", 6, 5)
{
	SetAction(IDLE);

	CreateAnimations();
	animations[curType]->Play();

	traceRange = new Circle(Vector2(), TRACE_RANGE);
	attackRange = new Rect(Vector2(), Vector2(200, 100));

	destPos = pos = { CENTER_X + 100.0f, CENTER_Y };

	size = texture->GetSize();

	Texture* front = Texture::Add(L"Textures/Pixel/ProgressBar1.bmp");
	Texture* back = Texture::Add(L"Textures/Pixel/ProgressBar.bmp");
	hpBar = new ProgressBar(front, back);
}

Monster::~Monster()
{
	delete traceRange;
	delete attackRange;
	delete hpBar;
}

void Monster::Update()
{
	traceRange->pos = pos;
	attackRange->pos = pos;

	hpBar->pos = { pos.x, Top() };
	hpBar->SetValue(curHp/ maxHp);

	SetState();
	ActionState();
	SetAnimation();

	__super::Update();
}

void Monster::Render(HDC hdc)
{
	HBRUSH nBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	SelectObject(hdc, nBrush);
	traceRange->Render(hdc);
	attackRange->Render(hdc);
	
	__super::Render(hdc);

	hpBar->Render(hdc);
}

void Monster::Damage()
{
	if (curType == HIT || curType == DEATH)
		return;

	isAttack = false;
	Audio::Get()->Play("Hit");
	SetAction(HIT);

	curHp -= 10.0f;
	if (curHp <= 0.0f)
	{
		state = State::DEATH;
		SetAction(DEATH);
	}
}

void Monster::Move()
{
	if (curType == ATTACK) return;
	if (curType == HIT) return;
	if (curType == DEATH) return;

	pos += velocity * speed * DELTA;
}

void Monster::SetState()
{
	if (state == State::HIT || state == State::DEATH) {
		return;
	}

	if (target == nullptr) {
		state = State::PATROL;
		return;
	}

	if (attackRange->IsCollision(target)) {
		state = State::ATTACK;
		destPos = pos;
	}
	else if (traceRange->IsCollision(target)) {
		state = State::TRACE;
	}
	else {
		state = State::PATROL;
	}

}

void Monster::ActionState()
{
	switch (state)
	{
	case Monster::State::PATROL:
		Patrol();
		break;
	case Monster::State::TRACE:
		Trace();
		break;
	case Monster::State::ATTACK:
		Attack();
		break;
	case Monster::State::HIT:
		Hit();
		break;
	case Monster::State::DEATH:
		Death();
		break;
	}
}

void Monster::SetAnimation()
{
	if (curType == ATTACK) return;
	if (curType == HIT) return;
	if (curType == DEATH) return;

	if (velocity.Length() > 0.0f) {
		SetAction(RUN);
	}
	else {
		SetAction(IDLE);
	}
}

void Monster::Patrol()
{
	float distance = (destPos- pos).Length();

	if (distance < 10.0f) {
		destPos.x = Random(-PATROL_RANGE, PATROL_RANGE) + pos.x;
		destPos.y = Random(-PATROL_RANGE, PATROL_RANGE) + pos.y;
		
		velocity = (destPos - pos).GetNormalized();
	}
}

void Monster::Trace()
{
	velocity = (target->pos - pos).GetNormalized();
}

void Monster::Attack()
{
	if (curType == HIT) return;
	if (curType == DEATH) return;

	velocity = {};

	SetAction(ATTACK);
}

void Monster::Hit()
{
}

void Monster::Death()
{
}

void Monster::CreateAnimations()
{
	animations[IDLE] = new Animation(texture->GetFrame());
	animations[IDLE]->SetPart(0, 3);


	animations[RUN] = new Animation(texture->GetFrame());
	animations[RUN]->SetPart(6, 9);

	int arr[] = { 12, 12, 12, 12, 12 };
	animations[HIT] = new Animation(texture->GetFrame());
	animations[HIT]->SetArray(arr, 5, false);
	animations[HIT]->SetEndEvent(bind(&Monster::EndAttack, this));

	animations[ATTACK] = new Animation(texture->GetFrame());
	animations[ATTACK]->SetPart(18, 21, false);
	animations[ATTACK]->SetEndEvent(bind(&Monster::EndAttack, this));

	animations[DEATH] = new Animation(texture->GetFrame());
	animations[DEATH]->SetPart(24, 29, false);
}
