#include "framework.h"
#include "Dominion/DominionScene.h"

GameManager::GameManager()
{
	Start();
	SceneManager::Get()->Add("DominionScene", new DominionScene);
	SceneManager::Get()->ChangeScene("DominionScene");
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
	PatBlt(hBackDC, 0, 0, WIN_WIDTH, WIN_HEIGHT, WHITENESS); //hBackDC�� 0, 0���� WIN_WIDTH, WIN_HEIGHT���� ������� ä���
	//InvalidateRect(hWnd, nullptr, true);            //��¸޼��� ȣ��

	SceneManager::Get()->Render(hBackDC);

	SetBkMode(hBackDC, 0);

	EffectManager::Get()->Render(hBackDC);
	Timer::Get()->Render(hBackDC);

	//hdc�� 0, 0���� WIN_WIDTH, WIN_HEIGHT���� hBackDC 0,0 ���� ���� �״�� �׸���)	
	BitBlt(hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT, hBackDC, 0, 0, SRCCOPY);	
}
