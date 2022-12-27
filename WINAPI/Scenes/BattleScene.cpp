#include "framework.h"
#include "BattleScene.h"

BattleScene::BattleScene()
{
	fighter = new Fighter();
	fighter->pos = { CENTER_X, CENTER_Y };
	monster = new Monster();
	monster->SetTarget(fighter);

	Audio::Get()->Add("BG", "Sounds/drunloop.wav", true);
	Audio::Get()->Play("BG");
}

BattleScene::~BattleScene()
{
	delete fighter;
	delete monster;
}

void BattleScene::Init()
{
}

void BattleScene::Update()
{
	if (fighter->GetAttackCollider()->IsRectCollision(monster)) {
		monster->Damage();
	}

	fighter->Update();
	monster->Update();
}

void BattleScene::Render(HDC hdc)
{
	fighter->Render(hdc);
	monster->Render(hdc);
}
