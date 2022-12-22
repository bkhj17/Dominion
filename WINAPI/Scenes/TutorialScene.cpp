#include "Framework.h"

TutorialScene::TutorialScene()
{
    rect = new Rect(Vector2((long)CENTER_X, (long)CENTER_Y), Vector2(50, 50));
    hBrush = CreateSolidBrush(RGB(0, 255, 255));
}

TutorialScene::~TutorialScene()
{
    delete rect;
    DeleteObject(hBrush);
}

void TutorialScene::Init()
{
}

void TutorialScene::Update()
{
    RECT winSize;
    GetClientRect(GameManager::Get()->GetHWND(), &winSize);

    if (GetAsyncKeyState(VK_RIGHT)) {
        rect->pos.x += speed;
        if (rect->pos.x > winSize.right - rect->GetSize().x / 2)
            rect->pos.x = winSize.right - rect->GetSize().x / 2;
    }
    if (GetAsyncKeyState(VK_LEFT)) {
        rect->pos.x -= speed;
        if (rect->pos.x < rect->GetSize().x / 2)
            rect->pos.x = rect->GetSize().x / 2;
    }
    if (GetAsyncKeyState(VK_UP)) {
        rect->pos.y -= speed;
        if (rect->pos.y < rect->GetSize().y / 2)
            rect->pos.y = rect->GetSize().y / 2;
    }
    if (GetAsyncKeyState(VK_DOWN)) {
        rect->pos.y += speed;
        if (rect->pos.y > winSize.bottom - rect->GetSize().y / 2)
            rect->pos.y = winSize.bottom - rect->GetSize().y / 2;
    }
}

void TutorialScene::Render(HDC hdc)
{
    auto origin = SelectObject(hdc, hBrush);
    rect->Render(hdc);
    SelectObject(hdc, origin);
}
