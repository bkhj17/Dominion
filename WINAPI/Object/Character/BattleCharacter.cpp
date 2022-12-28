#include "framework.h"
#include "BattleCharacter.h"

BattleCharacter::BattleCharacter(wstring file, int frameX, int frameY, COLORREF trans)
	: Character(file, frameX, frameY, trans)
{
	attackCollider = new Rect;
	attackCollider->isActive = false;
}

BattleCharacter::~BattleCharacter()
{
	delete attackCollider;
}

void BattleCharacter::Update()
{
	Move();
	SetAnimation();
	__super::Update();
}

void BattleCharacter::Render(HDC hdc)
{
	__super::Render(hdc);
	attackCollider->LineRender(hdc);
}

void BattleCharacter::Move()
{
	pos += velocity * DELTA;
}

void BattleCharacter::EndAttack()
{
	isAttack = false;
	attackCollider->isActive = false;

	SetAction(0);
}