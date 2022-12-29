#include "framework.h"
#include "BitmapScene.h"
#include "Framework.h"

BitmapScene::BitmapScene()
{
	HDC hdc = GetDC(hWnd);
	memDC = CreateCompatibleDC(hdc);				//hdc 크기의 memDC 생성
	ReleaseDC(hWnd, hdc);	//참조 종료
	testBitmap = (HBITMAP)LoadImage(
		hInst,						//창 생성시 만들어진 인스턴스
		L"Textures/Card.bmp",		//파일 경로
		IMAGE_BITMAP,				//파일 유형
		0, 0,					//비트맵 크기(이미지랑 다르면 확대, 축소 발생)
		LR_LOADFROMFILE);			//파일에서 불러온다고 선언
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
		50, 50,		//출력 위치
		200, 318,	//출력 크기
		memDC,		//복사 영역
		0, 0,		//복사 위치
		SRCCOPY		//복사
	);
	SelectObject(memDC, sivaBitmap);


	//StretchBlt(
	//	hdc,				//출력할 영역
	//	300, 0,				//출력 위치
	//	448 / 8, 104 / 2,	//출력 크기
	//	memDC,				//복사해올 영역
	//	0, 0,				//복사위치
	//	448 / 8, 104 / 2,	//복사할 크기
	//	SRCCOPY
	//);
	GdiTransparentBlt(
		hdc,				//출력할 영역
		300, 0,				//출력 위치
		448 / 8, 104 / 2,	//출력 크기
		memDC,				//복사해올 영역
		0, 0,				//복사위치
		448 / 8, 104 / 2,	//복사할 크기
		MAGENTA				//무시할 색(배경)
	);
}
