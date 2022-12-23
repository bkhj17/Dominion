#include "framework.h"

LandScape::LandScape()
{
	for (int i = 0; i < 2; i++) {
		ImageRect* bg = new ImageRect(L"Textures/background.bmp");
		bg->pos = { CENTER_X + WIN_WIDTH*i, CENTER_Y };
		//bg->size = { WIN_WIDTH, WIN_HEIGHT };
		bgs.push_back(bg);
	}
}

LandScape::~LandScape()
{
	for (auto bg : bgs) {
		delete bg;
	}
}

void LandScape::Update()
{ 
	for (auto bg : bgs) {
		bg->pos.x -= bgSpeed * DELTA;

		if (bg->Right() <= 0.0f) {
			bg->pos.x += WIN_WIDTH * bgs.size();
		}
	}
}

void LandScape::Render(HDC hdc)
{
	for (auto bg : bgs)
		bg->Render(hdc);
}
