#include "framework.h"
#include "BrickOutScene.h"

BrickOutScene::BrickOutScene()
{
	controlBar = new ControlBar;

	ball = new Ball;
	ball->SetBarRect(controlBar);

	BrickManager::Get()->LoadStage("TextData\\BrickOut\\Stage1.csv");
}

BrickOutScene::~BrickOutScene()
{
	delete controlBar;

	BrickManager::Delete();
}

void BrickOutScene::Start()
{
}

void BrickOutScene::Update()
{
	controlBar->Update();
	ball->Update();

	BrickManager::Get()->Update();
}

void BrickOutScene::Render(HDC hdc)
{
	controlBar->Render(hdc);
	ball->Render(hdc);

	BrickManager::Get()->Render(hdc);
}
