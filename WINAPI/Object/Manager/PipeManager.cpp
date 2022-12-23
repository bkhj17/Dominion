#include "framework.h"

PipeManager::PipeManager()
{
	Texture* upPipeTexture = Texture::Add(L"Textures/pipeup.bmp");
	Texture* downPipeTexture = Texture::Add(L"Textures/pipedown.bmp");

	upPipes.resize(PIPE_NUM);
	downPipes.resize(PIPE_NUM);

	for (UINT i = 0; i < PIPE_NUM; i++) {
		upPipes[i] = new ImageRect(L"Textures/pipeup.bmp");
		downPipes[i] = new ImageRect(L"Textures/pipedown.bmp");

		upPipes[i]->pos.x = downPipes[i]->pos.x = WIN_WIDTH + interval.x * i;
		upPipes[i]->pos.y = GameMath::Random(-upPipes[i]->size.y, CENTER_Y);
		downPipes[i]->pos.y = upPipes[i]->pos.y + upPipes[i]->size.y + interval.y;
	}

}

PipeManager::~PipeManager()
{
	for (UINT i = 0; i < PIPE_NUM; i++) {
		delete upPipes[i];
		delete downPipes[i];
	}

	upPipes.clear();
	downPipes.clear();
}

void PipeManager::Update()
{
	for (UINT i = 0; i < PIPE_NUM; i++) {
		upPipes[i]->pos.x += -speed * DELTA;
		downPipes[i]->pos.x += -speed * DELTA;

		if (upPipes[i]->Right() < 0.0f) {
			upPipes[i]->pos.x += interval.x * PIPE_NUM;
			downPipes[i]->pos.x = upPipes[i]->pos.x;
		}
	}
}

void PipeManager::Render(HDC hdc)
{
	for (UINT i = 0; i < PIPE_NUM; i++) {
		upPipes[i]->Render(hdc);
		downPipes[i]->Render(hdc);
	}
}

bool PipeManager::Collision(Rect* rect)
{

	for (UINT i = 0; i < PIPE_NUM; i++) {
		if (upPipes[i]->IsRectCollision(rect) 
			|| downPipes[i]->IsRectCollision(rect))
			return true;

	}
	return false;
}
