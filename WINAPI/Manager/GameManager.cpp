#include "framework.h"
#include "Scenes/TutorialScene.h"
#include "Scenes/HomeworkScene221219.h"
GameManager* GameManager::instance = nullptr;

GameManager::GameManager()
{
	scene = new HomeworkScene221219();
}

GameManager::~GameManager()
{
	delete scene;
}

void GameManager::Init(HWND hWnd)
{
	this->hWnd = hWnd;

	hdc = GetDC(hWnd);

	hBackDC = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, WIN_WIDTH, WIN_HEIGHT);
	SelectObject(hBackDC, hBitmap);
}

void GameManager::Update()
{
	scene->Update();
}

void GameManager::Render()
{
	PatBlt(hBackDC, 0, 0, WIN_WIDTH, WIN_HEIGHT, WHITENESS); //hBackDC의 0, 0에서 WIN_WIDTH, WIN_HEIGHT까지 흰색으로 채우기
	scene->Render(hBackDC);
	//InvalidateRect(hWnd, nullptr, true);            //출력메세지 호출

	//hdc의 0, 0에서 WIN_WIDTH, WIN_HEIGHT까지 hBackDC 0,0 부터 원본 그대로 그리기)	
	BitBlt(hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT, hBackDC, 0, 0, SRCCOPY);	
}
