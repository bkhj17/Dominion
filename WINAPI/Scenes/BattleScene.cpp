#include "framework.h"
#include "BattleScene.h"

BattleScene::BattleScene()
{
	Audio::Get()->Add("BG", "Sounds/drunloop.wav", true);
}

BattleScene::~BattleScene()
{
}

void BattleScene::Start()
{
	fighter = new Fighter();
	fighter->pos = { CENTER_X, CENTER_Y };
	monster = new Monster();
	monster->SetTarget(fighter);
	Audio::Get()->Play("BG");
}

void BattleScene::Update()
{
	if (fighter->GetAttackCollider()->IsRectCollision(monster)) {
		monster->Damage();
	}

	fighter->Update();
	monster->Update();

	if (KEY_DOWN('1')) {
		SceneManager::Get()->ChangeScene("DigGame");
		return;
	}
}

void BattleScene::Render(HDC hdc)
{
	fighter->Render(hdc);
	monster->Render(hdc);
}

void BattleScene::End()
{
	delete fighter;
	delete monster;
}
