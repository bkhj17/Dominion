#include "framework.h"
#include "CookieRunScene.h"

CookieRunScene::CookieRunScene()
{
	StageManager::Get()->LoadTileData(L"TextData/CookieRun/TileTable.csv");
	landscape = new LandScape(L"Textures/CookieRun/Background.bmp");

	cookie = new Cookie;
	cookie->pos = { 200.0f, CENTER_Y };
}

CookieRunScene::~CookieRunScene()
{
	StageManager::Delete();
	delete landscape;
	delete cookie;
}

void CookieRunScene::Start()
{
}

void CookieRunScene::Update()
{
	landscape->SetSpeed(StageManager::Get()->GetSpeed() * 0.5f);
	StageManager::Get()->Update();
	StageManager::Get()->CollisionCoin(cookie);

	landscape->Update();
	cookie->Update();
}

void CookieRunScene::Render(HDC hdc)
{
	landscape->Render(hdc);
	StageManager::Get()->Render(hdc);

	cookie->Render(hdc);
}
