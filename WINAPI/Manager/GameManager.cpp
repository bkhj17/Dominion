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
	PatBlt(hBackDC, 0, 0, WIN_WIDTH, WIN_HEIGHT, WHITENESS); //hBackDC�� 0, 0���� WIN_WIDTH, WIN_HEIGHT���� ������� ä���
	scene->Render(hBackDC);
	//InvalidateRect(hWnd, nullptr, true);            //��¸޼��� ȣ��

	//hdc�� 0, 0���� WIN_WIDTH, WIN_HEIGHT���� hBackDC 0,0 ���� ���� �״�� �׸���)	
	BitBlt(hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT, hBackDC, 0, 0, SRCCOPY);	
}
