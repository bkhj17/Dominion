#include "framework.h"
#include "CollisionScene.h"

CollisionScene::CollisionScene()
{
	Vector2 center(WIN_WIDTH/2, WIN_HEIGHT/2);

	hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
	hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));

	//objects.push_back(new Rect(center, { 50.0f, 50.0f }));
	objects.push_back(new Line(Vector2(50.0f, 50.0f), Vector2()));
	objects.push_back(new Circle(Vector2(700.0f, 50.0f), 360.0f));
	objects.push_back(new Circle({}, 10.0f));

	hRedPen = CreatePen(PS_SOLID, 5, RED);
	hBluePen = CreatePen(PS_SOLID, 5, BLUE);
}

CollisionScene::~CollisionScene()
{
	for(auto object : objects)
		delete object;
	DeleteObject(hRedBrush);
	DeleteObject(hBlueBrush);


	DeleteObject(hRedPen);
	DeleteObject(hBluePen);
}

void CollisionScene::Start()
{
}

void CollisionScene::Update()
{
	isCollision = objects[0]->IsCollision(objects[1]);
}

void CollisionScene::Render(HDC hdc)
{
	if (isCollision) {
		SelectObject(hdc, hRedBrush);
		SelectObject(hdc, hRedPen);
	}
	else {
		SelectObject(hdc, hBlueBrush);
		SelectObject(hdc, hBluePen);
	}
	objects[0]->Render(hdc);
	objects[1]->Render(hdc);
}
