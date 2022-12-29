#include "framework.h"
#include "HomeworkScene221221.h"

HomeworkScene221221::HomeworkScene221221()
{
    bar = new Bar1221(Vector2(CENTER_X, WIN_HEIGHT - 50), Vector2(100, 20));
    ball = new Ball1221;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 10; j++) {
            BlockManager1221::Get()->SpawnObject(Vector2(Block1221::SIZE_X * (0.5f + j), 150.0f + Block1221::SIZE_Y*i));
        }
    }

}

HomeworkScene221221::~HomeworkScene221221()
{
    delete bar;
    delete ball;
    BlockManager1221::Delete();
}

void HomeworkScene221221::Start()
{
    
    ball->pos = bar->GetBarrelPos();
}

void HomeworkScene221221::Update()
{
    switch (gameState)
    {
    case HomeworkScene221221::GameState::READY:
    {
        bar->Update();
        UpdateReady();
        if (!bar->isHoldBall) {
            gameState = GameState::PLAY;
        }
    }
        break;
    case HomeworkScene221221::GameState::PLAY:
    {
        UpdateGame();

        if (ball->pos.y - ball->GetRadius() > WIN_HEIGHT) {
            gameState = GameState::GAME_OVER;
            Timer::Get()->SetDeltaScale(0.0f);
            break;
        }
        if (BlockManager1221::Get()->Count() == 0) {
            gameState = GameState::CLEAR;
            Timer::Get()->SetDeltaScale(0.0f);
            break;
        }
    }
        break;
    case HomeworkScene221221::GameState::CLEAR:
    case HomeworkScene221221::GameState::GAME_OVER:
        if (KEY_DOWN(VK_SPACE)) {
            PostQuitMessage(0);
        }
        break;
    }


    bar->Update();
    if (bar->isHoldBall)
        UpdateReady();
    else {
        UpdateGame();
    }
}

void HomeworkScene221221::Render(HDC hdc)
{
    ball->Render(hdc);
    bar->Render(hdc);

    BlockManager1221::Get()->Render(hdc);


    if (gameState == GameState::GAME_OVER) {
        wstring str = L"GAME OVER";
        TextOut(hdc, 
            (int)(CENTER_X - 5.0f * str.size()), 
            (int)CENTER_Y - 10,
            str.c_str(), 
            (int)str.size());

    }
    else if (gameState == GameState::CLEAR) {
        wstring str = L"CLEAR";
        TextOut(hdc, 
            (int)(CENTER_X - 5.0f * str.size()),
            (int)CENTER_Y-10, 
            str.c_str(), 
            (int)str.size());
    }
}

void HomeworkScene221221::UpdateReady()
{
    ball->pos = bar->GetBarrelPos();

    if (KEY_DOWN(VK_SPACE)) {
        bar->isHoldBall = false;
        ball->distance = bar->GetBarrelDist();
    }
}

void HomeworkScene221221::UpdateGame()
{
    bar->Update();
    ball->Update();

    Vector2 overlap;
    ball->CollisionToRect(bar);

    BlockManager1221::Get()->Collision(ball);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

HomeworkScene221221::Bar1221::Bar1221(Vector2 pos, Vector2 size)
    : Rect(pos, size)
{
    barrelPos = Vector2(0.0f, -Ball1221::SIZE-size.y*0.5f);
    barrel = new Line;

    isHoldBall = true;

    hBrush = CreateSolidBrush(RGB(255, 255, 3));
}

HomeworkScene221221::Bar1221::~Bar1221()
{
    DeleteObject(hBrush);
}


void HomeworkScene221221::Bar1221::Update()
{
    if (KEY_PRESS(VK_RIGHT)) {
        pos.x += speed * DELTA;
        if (pos.x > WIN_WIDTH - size.x * 0.5f)
            pos.x = WIN_WIDTH - size.x * 0.5f;
    }
    if (KEY_PRESS(VK_LEFT)) {
        pos.x -= speed * DELTA;
        if (pos.x < size.x * 0.5f)
            pos.x = size.x * 0.5f;
    }

    if (KEY_PRESS(VK_UP))
        angle += DELTA;

    if (KEY_PRESS(VK_DOWN))
        angle -= DELTA;

    barrel->end = barrel->pos = pos+barrelPos;
    barrel->end.x += cosf(angle) * barrelLength;
    barrel->end.y += -sinf(angle) * barrelLength;
}

void HomeworkScene221221::Bar1221::Render(HDC hdc)
{
    SelectObject(hdc, hBrush);

    Rect::Render(hdc);

    if (isHoldBall)
        barrel->Render(hdc);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

HomeworkScene221221::Ball1221::Ball1221(const float size)
{
    radius = size;
    hBrush = CreateSolidBrush(RGB(155, 155, 255));
}

HomeworkScene221221::Ball1221::~Ball1221()
{
    DeleteObject(hBrush);
}

void HomeworkScene221221::Ball1221::Update() {
    pos += distance * speed * DELTA;
    BoundToWar();
}

void HomeworkScene221221::Ball1221::BoundToWar()
{
    if (pos.x < radius) {
        pos.x = radius;
        distance.x = abs(distance.x);
    }
    if (pos.x > WIN_WIDTH - radius) {
        pos.x = WIN_WIDTH - radius;
        distance.x = -abs(distance.x);
    }
    if (pos.y < radius) {
        pos.y = radius;
        distance.y = abs(distance.y);
    }
}

void HomeworkScene221221::Ball1221::Render(HDC hdc)
{
    SelectObject(hdc, hBrush);
    __super::Render(hdc);
}

bool HomeworkScene221221::Ball1221::CollisionToRect(Rect* rect)
{
    if (!IsCollision(rect))
        return false;

    Rect boundary(pos.x - radius, pos.y - radius, pos.x + radius, pos.y + radius);
    Vector2 overlap;
    if(!boundary.IsRectCollision(rect, &overlap))
        return false;
    if(overlap.x < radius && overlap.y < radius){
        //모서리 충돌
        int fx = distance.x > 0 ? 1 : -1;
        pos.x += -fx * overlap.x;             //
        int fy = distance.y > 0 ? 1 : -1;
        pos.y += fy * overlap.y;             // 

        Vector2 v = (pos - rect->pos);
        distance.x = -distance.x + v.x;
        distance.y = -distance.y + v.y;
        distance.Normalize();
    }
    else if (overlap.x < overlap.y || overlap.y == rect->size.y) {
        //좌우 충돌
        int f = distance.x > 0 ? 1 : -1;

        pos.x += -f * overlap.x;             //
        distance.x = -f * abs(distance.x);   //   
    }
    else {   
        //상하 충돌
        int f = distance.y > 0 ? 1 : -1;

        pos.y += f * overlap.y;             //
        distance.y = -f * abs(distance.y);   //
    }
    return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////


const float HomeworkScene221221::Block1221::SIZE_X = WIN_WIDTH / 10.0f;
const float HomeworkScene221221::Block1221::SIZE_Y = 100.0f;

HomeworkScene221221::Block1221::Block1221()
{
    size.x = SIZE_X;
    size.y = SIZE_Y;

    hBrush = CreateSolidBrush(RGB(
        GameMath::Random(0, 255),
        GameMath::Random(0, 255), 
        GameMath::Random(0, 255)));
}

HomeworkScene221221::Block1221::~Block1221()
{
    DeleteObject(hBrush);
}

void HomeworkScene221221::Block1221::Render(HDC hdc)
{
    SelectObject(hdc, hBrush);
    __super::Render(hdc);
}

void HomeworkScene221221::Block1221::Spawn(const Vector2& pos) {
    isActive = true;
    this->pos = pos;
}

void HomeworkScene221221::Block1221::Destroy()
{
    isActive = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

HomeworkScene221221::BlockManager1221::BlockManager1221()
{
    CreateObjects("Block", 20);
}


void HomeworkScene221221::BlockManager1221::SpawnObject(const Vector2& pos)
{

    Block1221* block = (Block1221*)Pop("Block");
    if (block != nullptr) {
        block->Spawn(pos);
    }
}

int HomeworkScene221221::BlockManager1221::Count()
{
    vector<GameObject*> objects = totalObjects["Block"];

    int cnt = 0;
    for (auto object : objects) {
        if (object->isActive)
            cnt++;
    }

    return cnt;
}

void HomeworkScene221221::BlockManager1221::CreateObjects(string key, UINT poolSize)
{
    vector<GameObject*> objects(poolSize);
    for (auto& object : objects) {
        object = new Block1221;
        object->isActive = false;
        object->tag = key;
    }
    totalObjects[key] = objects;
}

void HomeworkScene221221::BlockManager1221::Collision(Ball1221 * ball)
{

    vector<GameObject*> objects = totalObjects["Block"];
    for (auto object : objects) {
        Block1221* block = (Block1221*)object;
        if (block->isActive && ball->CollisionToRect(block)) {
            block->Destroy();
            return;
        }
    }
}
