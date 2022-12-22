#include "framework.h"
#include "CollisionScene.h"

CollisionScene::CollisionScene()
{
	Vector2 center(WIN_WIDTH/2, WIN_HEIGHT/2);

	hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
	hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));

	objects.push_back(new Rect(center, { 50.0f, 50.0f }));
}

CollisionScene::~CollisionScene()
{
	delete objects[0];
	DeleteObject(hRedBrush);
	DeleteObject(hBlueBrush);
}

void CollisionScene::Init()
{
}

void CollisionScene::Update()
{
	isCollision = objects[0]->IsPointCollision(mousePos);
}

void CollisionScene::Render(HDC hdc)
{
	if (isCollision)
		SelectObject(hdc, hRedBrush);
	else
		SelectObject(hdc, hBlueBrush);

	objects[0]->Render(hdc);
}
