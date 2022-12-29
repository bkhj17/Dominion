#include "framework.h"
#include "FlappyBirdScene.h"

FlappyBirdScene::FlappyBirdScene()
{
	landScape = new LandScape(L"Textures/background.bmp");
	flappyBird = new FlappyBird();
	flappyBird->pos = { WIN_WIDTH * 0.2f, CENTER_Y };
	PipeManager::Get();
}

FlappyBirdScene::~FlappyBirdScene()
{
	delete flappyBird;
	delete landScape;
}

void FlappyBirdScene::Start()
{
}

void FlappyBirdScene::Update()
{
	landScape->Update();
	flappyBird->Update();

	PipeManager::Get()->Update();
	if (PipeManager::Get()->Collision(flappyBird->BodyRect())) {
		MessageBox(hWnd, L"Game Over", L"Game Over", MB_OK);
	}
}

void FlappyBirdScene::Render(HDC hdc)
{
	landScape->Render(hdc);
	flappyBird->Render(hdc);

	PipeManager::Get()->Render(hdc);
}
