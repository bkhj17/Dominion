#include "Framework.h"

TutorialScene::TutorialScene()
{
    cardRect = new Rect(Vector2((long)CENTER_X, (long)CENTER_Y), Vector2(50, 50));
    hBrush = CreateSolidBrush(RGB(0, 255, 255));
}

TutorialScene::~TutorialScene()
{
    delete cardRect;
    DeleteObject(hBrush);
}

void TutorialScene::Start()
{
}

void TutorialScene::Update()
{
    RECT winSize;
    GetClientRect(hWnd, &winSize);

    if (GetAsyncKeyState(VK_RIGHT)) {
        cardRect->pos.x += speed;
        if (cardRect->pos.x > winSize.right - cardRect->size.x / 2)
            cardRect->pos.x = winSize.right - cardRect->size.x / 2;
    }
    if (GetAsyncKeyState(VK_LEFT)) {
        cardRect->pos.x -= speed;
        if (cardRect->pos.x < cardRect->size.x / 2)
            cardRect->pos.x = cardRect->size.x / 2;
    }
    if (GetAsyncKeyState(VK_UP)) {
        cardRect->pos.y -= speed;
        if (cardRect->pos.y < cardRect->size.y / 2)
            cardRect->pos.y = cardRect->size.y / 2;
    }
    if (GetAsyncKeyState(VK_DOWN)) {
        cardRect->pos.y += speed;
        if (cardRect->pos.y > winSize.bottom - cardRect->size.y / 2)
            cardRect->pos.y = winSize.bottom - cardRect->size.y / 2;
    }
}

void TutorialScene::Render(HDC hdc)
{
    auto origin = SelectObject(hdc, hBrush);
    cardRect->Render(hdc);
    SelectObject(hdc, origin);
}
