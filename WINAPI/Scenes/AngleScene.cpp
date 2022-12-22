#include "framework.h"
#include "AngleScene.h"

AngleScene::AngleScene()
{
	cannon = new Cannon();
	cannon->pos.x = CENTER_X;
	cannon->pos.y = CENTER_Y;
}

AngleScene::~AngleScene()
{
	delete cannon;
}

void AngleScene::Init()
{
}

void AngleScene::Update()
{
	cannon->Update();
}

void AngleScene::Render(HDC hdc)
{
	cannon->Render(hdc);
}
