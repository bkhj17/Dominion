#pragma once
class GameManager
{
private:
	GameManager();
	~GameManager();
public:
	static GameManager* Get() { if (instance == nullptr) instance = new GameManager; return instance; }
	static void Delete() { if (instance != nullptr) delete instance; }
	void Init(HWND hWnd);
	void Update();
	void Render();

	HWND GetHWND() { return hWnd; }
private:
	static GameManager* instance;

	Scene* scene = nullptr;

	HWND hWnd = nullptr;
	HDC hdc = nullptr;
	//백버퍼
	HDC hBackDC = nullptr;		//메모리
	HBITMAP hBitmap = nullptr;	//그릴 공간
};

