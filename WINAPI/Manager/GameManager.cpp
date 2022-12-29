#include "framework.h"
//#include "Scenes/TutorialScene.h"
//#include "Scenes/HomeworkScene221219.h"
//#include "Scenes/HomeworkScene221220.h"
//#include "Scenes/HomeworkScene221221.h"
//#include "Scenes/HomeworkScene221222.h"
//#include "Scenes/Homework221223.h"
//#include "Scenes/Homework221226.h"
//#include "Scenes/Homework/Homework221228.h"
#include "Scenes/Homework/Homework221229/Homework221229.h"
#include "Scenes/Homework/Homework221229/ShopScene1229.h"
#include "Scenes/Homework/Homework221229/SellScene1229.h"
#include "Scenes/Homework/Homework221229/InventoryScene1229.h"
//#include "Scenes/SpawnScene.h"
//#include "Scenes/CollisionScene.h"
//#include "Scenes/ShootingScene.h"
//#include "Scenes/AngleScene.h"
//#include "Scenes/BrickOutScene.h"
//#include "Scenes/BitmapScene.h"
//#include "Scenes/FlappyBirdScene.h"
//#include "Scenes/CookieRunScene.h"
#include "Scenes/BattleScene.h"
//#include "Scenes/PixelCollisionScene.h"
#include "Scenes/DigGameScene.h"
#include "Scenes/ButtonScene.h"
//#include "MyFramework/TestScene.h"

GameManager::GameManager()
{
	//Timer::Get()->SetLockFPS(1000);

	Start();
	SceneManager::Get()->Add("Homwork1229", new Homework221229);
	SceneManager::Get()->Add("Shop1229", new ShopScene1229);
	SceneManager::Get()->Add("Sell1229", new SellScene1229);
	SceneManager::Get()->Add("Inventory1229", new InventoryScene1229);
	SceneManager::Get()->ChangeScene("Homwork1229");
}

GameManager::~GameManager()
{
	Timer::Delete();
	Texture::Delete();
	Audio::Delete();
	EffectManager::Delete();
	Camera::Delete();
	Observer::Delete();
	SceneManager::Delete();
}

void GameManager::Start()
{
	hdc = GetDC(hWnd);
	hBackDC = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, WIN_WIDTH, WIN_HEIGHT);
	SelectObject(hBackDC, hBitmap);

	Timer::Get();
	KeyBoard::Get();
	Audio::Get();
	EffectManager::Get();
	Camera::Get();
	Observer::Get();
	SceneManager::Get();
}

void GameManager::Update()
{
	if (KEY_DOWN(VK_TAB))
		Texture::SetDebug();

	Timer::Get()->Update();
	KeyBoard::Get()->Update();
	Audio::Get()->Update();
	EffectManager::Get()->Update();
	SceneManager::Get()->Update();

	Camera::Get()->Update();
}

void GameManager::Render()
{
	PatBlt(hBackDC, 0, 0, WIN_WIDTH, WIN_HEIGHT, WHITENESS); //hBackDC의 0, 0에서 WIN_WIDTH, WIN_HEIGHT까지 흰색으로 채우기
	//InvalidateRect(hWnd, nullptr, true);            //출력메세지 호출

	SceneManager::Get()->Render(hBackDC);
	EffectManager::Get()->Render(hBackDC);
	Timer::Get()->Render(hBackDC);

	//hdc의 0, 0에서 WIN_WIDTH, WIN_HEIGHT까지 hBackDC 0,0 부터 원본 그대로 그리기)	
	BitBlt(hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT, hBackDC, 0, 0, SRCCOPY);	
}
