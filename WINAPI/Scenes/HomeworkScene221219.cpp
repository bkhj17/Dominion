#include "framework.h"
#include "HomeworkScene221219.h"

HomeworkScene221219::HomeworkScene221219()
{
	vRect.resize(MAX_RECT);
	for (int i = 0; i < MAX_RECT; i++) {
		vRect[i] = new VanishingRect;
	}
}

HomeworkScene221219::~HomeworkScene221219()
{
	for (int i = 0; i < vRect.size(); i++) {
		if (vRect[i] != nullptr)
			delete vRect[i];
	}
}

void HomeworkScene221219::Init()
{
}

void HomeworkScene221219::Update()
{
	clock_t now = clock();

	for (int i = 0; i < nShow; i++) {
		if (vRect[i]->VanishTime() <= now) {
			if (--nShow > 0) {
				VanishingRect* tmp = vRect[i];
				vRect[i] = vRect[nShow];
				vRect[nShow] = tmp;
			}
		}
	}

	if (nextShowTime <= now) {
		if (nShow < MAX_RECT) {
			nextShowTime = now + rand() % (MAX_SHOW_TIME-MIN_SHOW_TIME)+ MIN_SHOW_TIME;
			vRect[nShow++]->Init();
		}
	}
}

void HomeworkScene221219::Render(HDC hdc)
{
	for (int i = 0; i != nShow; (++i) %= MAX_RECT) {
		vRect[i]->Render(hdc);
	}
}

HomeworkScene221219::VanishingRect::VanishingRect()
{
}

HomeworkScene221219::VanishingRect::~VanishingRect()
{
	DeleteObject(this->brush);
}

void HomeworkScene221219::VanishingRect::Init()
{
	shape = (SHAPE)(rand() % 2);

	GetClientRect(hWnd, &winSize);
	
	if (brush != nullptr)
		DeleteObject(brush);
	brush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));

	lane;
	
	lane.pos.x = Random(lane.size.x * 0.5f, winSize.right - lane.size.x*0.5f);
	lane.pos.y = Random(lane.size.y * 0.5f, winSize.right - lane.size.y * 0.5f);
	vanishTime = clock() + rand()%(MAX_VANISH_TIME-MIN_VANISH_TIME) + MIN_VANISH_TIME;

}

void HomeworkScene221219::VanishingRect::Update()
{
}

void HomeworkScene221219::VanishingRect::Render(HDC hdc)
{

	if (shape == SHAPE::RECT) {
		auto origin = SelectObject(hdc, brush);
		lane.Render(hdc);
		SelectObject(hdc, origin);
	}
	else {
		int left = (int)(lane.pos.x - lane.size.x *0.5f);
		int right = (int)(lane.pos.x + lane.size.x * 0.5f);
		int top = (int)(lane.pos.y - lane.size.y * 0.5f);
		int bottom = (int)(lane.pos.y + lane.size.y * 0.5f);

		if (shape == SHAPE::ELLIPSE) {
			auto origin = SelectObject(hdc, brush);
			Ellipse(hdc, left, top, right, bottom);
			SelectObject(hdc, origin);
		}
	}
	

}
