#include "framework.h"

LandScape::LandScape(wstring file)
{
	for (int i = 0; i < 2; i++) {
		ImageRect* bg = new ImageRect(file);
		bg->pos = { bg->Half().x + bg->size.x * i,  bg->Half().y };
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
			bg->pos.x += bg->size.x * bgs.size();
		}
	}
}

void LandScape::Render(HDC hdc)
{
	for (auto bg : bgs)
		bg->Render(hdc);
}
