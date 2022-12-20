#include "framework.h"
#include "Scenes/TutorialScene.h"
#include "Scenes/HomeworkScene221219.h"
#include "Scenes/HomeworkScene221220.h"
#include "Scenes/SpawnScene.h"
#include "Scenes/CollisionScene.h"

GameManager::GameManager()
{
	scene = new HomeworkScene221220();
	Timer::Get();
	KeyBoard::Get();
}

GameManager::~GameManager()
{
	delete scene;
	Timer::Get()->Delete();
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
	Timer::Get()->Update();
	KeyBoard::Get()->Update();
	scene->Update();
}

void GameManager::Render()
{
	PatBlt(hBackDC, 0, 0, WIN_WIDTH, WIN_HEIGHT, WHITENESS); //hBackDC�� 0, 0���� WIN_WIDTH, WIN_HEIGHT���� ������� ä���
	scene->Render(hBackDC);
	//InvalidateRect(hWnd, nullptr, true);            //��¸޼��� ȣ��
	Timer::Get()->Render(hBackDC);

	//hdc�� 0, 0���� WIN_WIDTH, WIN_HEIGHT���� hBackDC 0,0 ���� ���� �״�� �׸���)	
	BitBlt(hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT, hBackDC, 0, 0, SRCCOPY);	
}
