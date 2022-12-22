#include "Framework.h"

Plane::Plane() {
	radius = BODY_SIZE;
	hPen = CreatePen(PS_SOLID, 3, RGB(0,255,0));
	hBrush = CreateSolidBrush(RGB(100,100,100));

	pos.x = CENTER_X;
	pos.y = CENTER_Y;
}

Plane::~Plane()
{
	DeleteObject(hPen);
	DeleteObject(hBrush);

}

void Plane::Update()
{
	MoveControl();
	FireControl();

	Damage();
}

void Plane::Render(HDC hdc)
{
	SelectObject(hdc, hPen);
	SelectObject(hdc, hBrush);

	RenderBody(hdc);
}

void Plane::RenderBody(HDC hdc)
{
	Ellipse(hdc,
		(int)(pos.x - BODY_SIZE * 0.5f),
		(int)(pos.y - BODY_SIZE),
		(int)(pos.x + BODY_SIZE * 0.5f),
		(int)(pos.y + BODY_SIZE));

	Ellipse(hdc,
		(int)(pos.x - BODY_SIZE),
		(int)(pos.y + BODY_SIZE * 0.25f),
		(int)(pos.x + BODY_SIZE),
		(int)(pos.y + BODY_SIZE));
}

void Plane::MoveControl()
{
	if (KEY_PRESS(VK_RIGHT)) {
		pos.x += speed * DELTA;
		if (pos.x > WIN_WIDTH - BODY_SIZE)
			pos.x = WIN_WIDTH - BODY_SIZE;
	}
	if (KEY_PRESS(VK_LEFT)) {
		pos.x -= speed * DELTA;
		if (pos.x < BODY_SIZE)
			pos.x = BODY_SIZE;
	}
	if (KEY_PRESS(VK_UP)) {
		pos.y -= speed * DELTA;
		if (pos.y < BODY_SIZE)
			pos.y = BODY_SIZE;
	}
	if (KEY_PRESS(VK_DOWN)) {
		pos.y += speed * DELTA;
		if (pos.y > WIN_HEIGHT - BODY_SIZE)
			pos.y = WIN_HEIGHT - BODY_SIZE;
	}
}

void Plane::FireControl()
{
	if (KEY_DOWN(VK_SPACE)) {
		BulletManager::Get()->Fire(pos);
	}
}

void Plane::Damage()
{
	bool hit = BulletManager::Get()->Collision(this);
	if (hit) {
		Timer::Get()->SetDeltaScale(0.0f);

		MessageBox(hWnd, L"Game Over", L"GameOver", MB_OK);
		PostQuitMessage(0);
	}


	Enemy* enemy = EnemyManager::Get()->Collision(this);

	if (enemy && enemy != nullptr) {
		enemy->Destroy();

		Timer::Get()->SetDeltaScale(0.0f);

		MessageBox(hWnd, L"Game Over", L"GameOver", MB_OK);
		PostQuitMessage(0);
	}

}
