#include "framework.h"

ControlBar::ControlBar()
{
	pos = { CENTER_X, WIN_HEIGHT * 0.9 };
	size = { BAR_WIDTH, BAR_HEIGHT };

	hBrush = CreateSolidBrush(RGB( 100, 100, 100));
}

ControlBar::~ControlBar()
{
	DeleteObject(hBrush);
}

void ControlBar::Update()
{
	Control();
}

void ControlBar::Render(HDC hdc)
{
	SelectObject(hdc, hBrush);
	Rect::Render(hdc);
}

void ControlBar::Control()
{
	if (KEY_PRESS(VK_RIGHT)) {
		pos.x += speed * DELTA;

		if (Right() > WIN_WIDTH)
			pos.x = WIN_WIDTH - Half().x;
	}

	if (KEY_PRESS(VK_LEFT)) {
		pos.x -= speed * DELTA;

		if (Left() < 0)
			pos.x = Half().x;
	}

}
