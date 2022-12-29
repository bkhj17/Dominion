#include "framework.h"
#include "BitmapScene.h"
#include "Framework.h"

BitmapScene::BitmapScene()
{
	HDC hdc = GetDC(hWnd);
	memDC = CreateCompatibleDC(hdc);				//hdc ũ���� memDC ����
	ReleaseDC(hWnd, hdc);	//���� ����
	testBitmap = (HBITMAP)LoadImage(
		hInst,						//â ������ ������� �ν��Ͻ�
		L"Textures/Card.bmp",		//���� ���
		IMAGE_BITMAP,				//���� ����
		0, 0,					//��Ʈ�� ũ��(�̹����� �ٸ��� Ȯ��, ��� �߻�)
		LR_LOADFROMFILE);			//���Ͽ��� �ҷ��´ٰ� ����
	sivaBitmap = (HBITMAP)LoadImage(
		hInst,						
		L"Textures/SivaRun.bmp",	
		IMAGE_BITMAP,				
		0, 0,					
		LR_LOADFROMFILE
	);			


}

BitmapScene::~BitmapScene()
{
	DeleteObject(testBitmap);
	DeleteObject(sivaBitmap);
}

void BitmapScene::Start()
{
}

void BitmapScene::Update()
{
}

void BitmapScene::Render(HDC hdc)
{
	SelectObject(memDC, testBitmap);
	BitBlt(hdc,
		50, 50,		//��� ��ġ
		200, 318,	//��� ũ��
		memDC,		//���� ����
		0, 0,		//���� ��ġ
		SRCCOPY		//����
	);
	SelectObject(memDC, sivaBitmap);


	//StretchBlt(
	//	hdc,				//����� ����
	//	300, 0,				//��� ��ġ
	//	448 / 8, 104 / 2,	//��� ũ��
	//	memDC,				//�����ؿ� ����
	//	0, 0,				//������ġ
	//	448 / 8, 104 / 2,	//������ ũ��
	//	SRCCOPY
	//);
	GdiTransparentBlt(
		hdc,				//����� ����
		300, 0,				//��� ��ġ
		448 / 8, 104 / 2,	//��� ũ��
		memDC,				//�����ؿ� ����
		0, 0,				//������ġ
		448 / 8, 104 / 2,	//������ ũ��
		MAGENTA				//������ ��(���)
	);
}
