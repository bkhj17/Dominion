#pragma once
#include "Framework/Utilities/Singleton.h"

class GameManager : public Singleton<GameManager>
{
private:
	friend class Singleton;

	GameManager();
	~GameManager();
public:
	void Init(HWND hWnd);
	void Update();
	void Render();

	HWND GetHWND() { return hWnd; }

private:

	Scene* scene = nullptr;

	HWND hWnd = nullptr;
	HDC hdc = nullptr;
	//백버퍼
	HDC hBackDC = nullptr;		//메모리
	HBITMAP hBitmap = nullptr;	//그릴 공간
};

