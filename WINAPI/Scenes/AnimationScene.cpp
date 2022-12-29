#include "framework.h"
#include "AnimationScene.h"

AnimationScene::AnimationScene()
{
	megaman = new Megaman();
	megaman->pos.x = CENTER_X;
	megaman->pos.y = CENTER_Y;
}

AnimationScene::~AnimationScene()
{
	delete megaman;
}

void AnimationScene::Start()
{
}

void AnimationScene::Update()
{
	megaman->Update();
}

void AnimationScene::Render(HDC hdc)
{
	megaman->Render(hdc);
}
