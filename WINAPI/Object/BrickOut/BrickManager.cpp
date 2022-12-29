#include "framework.h"

BrickManager::BrickManager()
{
    hBrushes = new HBRUSH[MAX_HP];
    hBrushes[0] = CreateSolidBrush(WHITE);
    hBrushes[1] = CreateSolidBrush(BLUE);
    hBrushes[2] = CreateSolidBrush(GREEN);
    hBrushes[3] = CreateSolidBrush(YELLOW);
    hBrushes[4] = CreateSolidBrush(RED);
}

BrickManager::~BrickManager()
{
    DeleteObject(hBrushes[0]);
    DeleteObject(hBrushes[1]);
    DeleteObject(hBrushes[2]);
    DeleteObject(hBrushes[3]);
    DeleteObject(hBrushes[4]);
}

Direction BrickManager::Collision(GameObject* object)
{
    Circle* ball = (Circle*)object;

    vector<GameObject*> bricks = totalObjects[BRICK_KEY];
    
    Direction dir = Direction::NONE;
    for (GameObject* brick : bricks) {
        dir = ((Brick*)brick)->Collision(ball);
        if (dir != Direction::NONE)
            break;
    }

    return dir;
}

void BrickManager::LoadStage(string file)
{
    ClearStage();
    //csv
    ifstream loadFile;
    loadFile.open(file);

    while (1) {
        string temp;
        loadFile >> temp;
        vector<string> datas = SplitString(temp, ",");
        if (datas.empty())
            break;

        width = (UINT)datas.size();

        vector<UINT> rowDatas;
        rowDatas.reserve(width);

        for (string data : datas) {
            rowDatas.push_back(stoi(data));
        }
        stageData.push_back(rowDatas);
        height++;

        if (loadFile.eof())
            break;
    }
    loadFile.close();

    CreateObjects(BRICK_KEY, width * height);
}

void BrickManager::CreateObjects(string key, UINT poolSize)
{
    Vector2 size = { WIN_WIDTH / (float)width, WIN_HEIGHT / height * 0.3f };

    vector<GameObject*> bricks;
    bricks.reserve(poolSize);

    for (UINT y = 0; y < height; y++) {
        for (UINT x = 0; x < width; x++) {
            if (stageData[y][x] == 0)
                continue;

            Vector2 pos;
            pos.x = (x+0.5f) * size.x;
            pos.y = (y+0.5f) * size.y;

            Brick* brick = new Brick(pos, size, stageData[y][x]);
            brick->SetBrushes(hBrushes);
            bricks.push_back(brick);
        }
    }

    totalObjects[key] = bricks;
}

void BrickManager::ClearStage()
{
    ClearObjects(BRICK_KEY);
    for (vector<UINT> datas : stageData) {
        datas.clear();
    }
    stageData.clear();
}
