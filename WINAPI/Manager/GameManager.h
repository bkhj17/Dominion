#pragma once
#include "Framework/Utilities/Singleton.h"

class GameManager : public Singleton<GameManager>
{
private:
	friend class Singleton;

	GameManager();
	~GameManager();
public:
	void Start();
	void Update();
	void Render();

	HDC GetHDC() { return hdc; }
private:

	HDC hdc = nullptr;
	//�����
	HDC hBackDC = nullptr;		//�޸�
	HBITMAP hBitmap = nullptr;	//�׸� ����
};

