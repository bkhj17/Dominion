#include "Framework.h"

TutorialScene::TutorialScene()
{
    rect = new Rect(50, 50);
    rect->Pos() = { (long)CENTER_X, (long)CENTER_Y };

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
        rect->Pos().x += speed;
        if (rect->Pos().x > winSize.right - rect->GetSize().x / 2)
            rect->Pos().x = winSize.right - rect->GetSize().x / 2;
    }
    if (GetAsyncKeyState(VK_LEFT)) {
        rect->Pos().x -= speed;
        if (rect->Pos().x < rect->GetSize().x / 2)
            rect->Pos().x = rect->GetSize().x / 2;
    }
    if (GetAsyncKeyState(VK_UP)) {
        rect->Pos().y -= speed;
        if (rect->Pos().y < rect->GetSize().y / 2)
            rect->Pos().y = rect->GetSize().y / 2;
    }
    if (GetAsyncKeyState(VK_DOWN)) {
        rect->Pos().y += speed;
        if (rect->Pos().y > winSize.bottom - rect->GetSize().y / 2)
            rect->Pos().y = winSize.bottom - rect->GetSize().y / 2;
    }
}

void TutorialScene::Render(HDC hdc)
{
    auto origin = SelectObject(hdc, hBrush);
    rect->Render(hdc);
    SelectObject(hdc, origin);
}
