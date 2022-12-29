#include "framework.h"
#include "PixelCollisionScene.h"

PixelCollisionScene::PixelCollisionScene()
{
	land = new ImageRect(L"Textures/Pixel/land.bmp", 1, 1, RGB(0, 178, 255));
	land->pos = land->Half();
	background = new ImageRect(L"Textures/Pixel/background.bmp");
	background->size = land->size;
	background->pos = land->Half();

	megaman = new Megaman();
	megaman->pos = { CENTER_X, 0};
	megaman->SetLand(land->GetTexture());

	Camera::Get()->SetTarget(megaman);
	//Camera::Get()->SetOffset(0.0f, -300.0f);
	Camera::Get()->SetMapRect(land);
}

PixelCollisionScene::~PixelCollisionScene()
{
	delete land;
	delete background;

	delete megaman;
}

void PixelCollisionScene::Start()
{
}

void PixelCollisionScene::Update()
{
	megaman->Update();
}

void PixelCollisionScene::Render(HDC hdc)
{
	background->CamRender(hdc, {0,0}, false);
	land->CamRender(hdc);
	megaman->Render(hdc);
}