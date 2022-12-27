#include "framework.h"
//#include "Scenes/TutorialScene.h"
//#include "Scenes/HomeworkScene221219.h"
//#include "Scenes/HomeworkScene221220.h"
//#include "Scenes/HomeworkScene221221.h"
//#include "Scenes/HomeworkScene221222.h"
//#include "Scenes/Homework221223.h"
//#include "Scenes/Homework221226.h"
#include "Scenes/Homework221227.h"
//#include "Scenes/SpawnScene.h"
//#include "Scenes/CollisionScene.h"
//#include "Scenes/ShootingScene.h"
//#include "Scenes/AngleScene.h"
//#include "Scenes/BrickOutScene.h"
//#include "Scenes/BitmapScene.h"
//#include "Scenes/FlappyBirdScene.h"
#include "Scenes/CookieRunScene.h"
#include "Scenes/BattleScene.h"
//#include "MyFramework/TestScene.h"

GameManager::GameManager()
{
	//Timer::Get()->SetLockFPS(1000);

	Init();
	scene = new Homework221227();

}

GameManager::~GameManager()
{
	delete scene;
	Timer::Delete();
	Texture::Delete();
	Audio::Delete();
}

void GameManager::Init()
{
	hdc = GetDC(hWnd);
	hBackDC = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, WIN_WIDTH, WIN_HEIGHT);
	SelectObject(hBackDC, hBitmap);


	Timer::Get();
	KeyBoard::Get();
	Audio::Get();
	SetBkMode(hBackDC, TRANSPARENT);
	SetTextColor(hdc, WHITE);
}

void GameManager::Update()
{
	Timer::Get()->Update();
	KeyBoard::Get()->Update();
	Audio::Get()->Update();
	scene->Update();
}

void GameManager::Render()
{
	PatBlt(hBackDC, 0, 0, WIN_WIDTH, WIN_HEIGHT, WHITENESS); //hBackDC의 0, 0에서 WIN_WIDTH, WIN_HEIGHT까지 흰색으로 채우기
	scene->Render(hBackDC);
	//InvalidateRect(hWnd, nullptr, true);            //출력메세지 호출
	Timer::Get()->Render(hBackDC);

	//hdc의 0, 0에서 WIN_WIDTH, WIN_HEIGHT까지 hBackDC 0,0 부터 원본 그대로 그리기)	
	BitBlt(hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT, hBackDC, 0, 0, SRCCOPY);	
}
