#include "framework.h"

StageManager::StageManager()
{
    Play();
}

StageManager::~StageManager()
{
}

void StageManager::Update()
{
    if (!isPlay)
        return;

    for (auto& objects : totalObjects) {
        for (auto object : objects.second) {
            object->Update();

            object->pos.x -= speed * DELTA;
            Rect* cardRect = (Rect*)object;
            if (cardRect->Right() <= 0.0f) {
                cardRect->isActive = true;
                cardRect->pos.x += width * tileSize.x;
            }
        }
    }
}
float StageManager::GetGroundHeight(Vector2 pos)
{
    float minHeight = WIN_HEIGHT;
    for (auto object : totalObjects["Ground"]) {
        ImageRect* cardRect = (ImageRect*)object;
        if (pos.x < cardRect->Left() || pos.x > cardRect->Right())
            continue;

        if (pos.y >= cardRect->Top())
            continue;

        minHeight = min(cardRect->Top(), minHeight);
    }

    return minHeight;
}
bool StageManager::CollisionCoin(Rect* cardRect)
{
    for (auto coin : totalObjects["Coin"]) {
        if (!coin->isActive)
            continue;

        if(coin->IsRectCollision(cardRect)) {
            coin->isActive = false;
            score++;
            return true;
        }
    }

    return false;
}

bool StageManager::CollisionObstacle(Rect* cardRect)
{

    for (auto obstacle : totalObjects["Obstacle"]) {
        if (obstacle->IsCollision(cardRect)) {
            return true;
        }
    }

    return false;
}

void StageManager::Render(HDC hdc)
{
    for (auto ground : totalObjects["Ground"]) {
        ImageRect* cardRect = (ImageRect*)ground;
        cardRect->Render(hdc);
    }

    for (auto object : totalObjects["Coin"]) {
        Coin* coin = (Coin*)object;
        coin->Render(hdc);
    }

    for (auto obstacle : totalObjects["Obstacle"]) {
        ImageRect* cardRect = (ImageRect*)obstacle;
        cardRect->Render(hdc);
    }

    wstring str = L"Score : " + to_wstring(score);
    SetTextColor(hdc, WHITE);
    TextOut(hdc, 0, 100, str.c_str(), str.size());
}

void StageManager::LoadTileData(wstring file)
{
    ifstream loadFile;
    loadFile.open(file);

    while (1) {
        string temp;
        loadFile >> temp;
        vector<string> datas = SplitString(temp, ",");
        if (datas.empty())
            break;

        width = (UINT)datas.size();

        vector<TileType> rowDatas;
        tileData.reserve(width);

        for (string data : datas) {
            rowDatas.push_back(TileType(stoi(data)));
        }
        tileData.push_back(rowDatas);
        height++;

        if (loadFile.eof())
            break;
    }
    loadFile.close();

    tileSize = { static_cast<float>(WIN_WIDTH / width) * 2, static_cast<float>(WIN_HEIGHT / height) };
    CreateTile();
}

void StageManager::CreateTile()
{
    vector<GameObject*> grounds;
    vector<GameObject*> coins;
    vector<GameObject*> obstacles;

    Vector2 startOffset = tileSize * 0.5f;

    grounds.reserve(width);
    coins.reserve(width);
    obstacles.reserve(width);

    for (UINT y = 0; y < height; y++) {
        for (UINT x = 0; x < width; x++) {
            TileType type = tileData[y][x];
            switch (type)
            {
            case StageManager::GROUND:
            {
                ImageRect* ground = new ImageRect(L"Textures/CookieRun/Ground.bmp");
                ground->pos = { tileSize.x * (x + 0.5f) , tileSize.y * (y + 0.5f) };
                grounds.push_back(ground);
            }
                break;
            case StageManager::COIN:
            {
                Coin* coin = new Coin;
                coin->pos = { tileSize.x * (x + 0.5f) , tileSize.y * (y + 0.5f) };
                coins.push_back(coin);
                break;
            }
            case StageManager::OBSTACLE:
            {
                ImageRect* obstacle = new ImageRect(L"Textures/CookieRun/Obstacle.bmp");
                obstacle->pos = { tileSize.x * (x + 0.5f) , tileSize.y * (y + 0.5f) };
                obstacles.push_back(obstacle);
            }
                break;
            }
        }
    }

    grounds.shrink_to_fit();
    coins.shrink_to_fit();
    obstacles.shrink_to_fit();

    totalObjects["Ground"] = grounds;
    totalObjects["Coin"] = coins;
    totalObjects["Obstacle"] = obstacles;
}
