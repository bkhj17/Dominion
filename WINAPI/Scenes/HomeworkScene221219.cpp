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
	clock_t now = (clock()/50)%1000;

	if (nextVanishTime <= now) {
		if (nextShowRect != nextVanishRect) {
			nextVanishTime = (now + rand() % 2+2) % 1000;
			(++nextVanishRect) %= MAX_RECT;
		}
	}

	if (nextShowTime <= now) {
		if ((nextShowRect + 1) % MAX_RECT != nextVanishRect) {
			nextShowTime = (now + 1) % 1000;
			(++nextShowRect) %= MAX_RECT;
			vRect[nextShowRect]->Init();
		}
	}
}

void HomeworkScene221219::Render(HDC hdc)
{
	for (int i = nextVanishRect; i != nextShowRect; (++i) %= MAX_RECT) {
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

	GetClientRect(GameManager::Get()->GetHWND(), &winSize);

	if (brush != nullptr)
		DeleteObject(brush);
	brush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));

	rect = Rect(rand() % 100 + 10, rand() % 100 + 10);
	rect.Pos().x = rand() % (winSize.right-rect.GetSize().x)+rect.GetSize().x/2;
	rect.Pos().y = rand() % (winSize.bottom-rect.GetSize().y) + rect.GetSize().y / 2;

}

void HomeworkScene221219::VanishingRect::Update()
{
}

void HomeworkScene221219::VanishingRect::Render(HDC hdc)
{

	if (shape == SHAPE::RECT) {
		auto origin = SelectObject(hdc, brush);
		rect.Render(hdc);
		SelectObject(hdc, origin);
	}
	else {
		int left = rect.Pos().x - rect.GetSize().x / 2;
		int right = rect.Pos().x + rect.GetSize().x / 2;
		int top = rect.Pos().y - rect.GetSize().y / 2;
		int bottom = rect.Pos().y + rect.GetSize().y / 2;

		if (shape == SHAPE::ELLIPSE) {
			auto origin = SelectObject(hdc, brush);
			Ellipse(hdc, left, top, right, bottom);
			SelectObject(hdc, origin);
		}
	}
	

}
