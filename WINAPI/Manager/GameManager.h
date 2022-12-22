#pragma once
#include "Framework/Utilities/Singleton.h"

class GameManager : public Singleton<GameManager>
{
private:
	friend class Singleton;

	GameManager();
	~GameManager();
public:
	void Init();
	void Update();
	void Render();

private:
	Scene* scene = nullptr;

	HDC hdc = nullptr;
	//�����
	HDC hBackDC = nullptr;		//�޸�
	HBITMAP hBitmap = nullptr;	//�׸� ����
};

