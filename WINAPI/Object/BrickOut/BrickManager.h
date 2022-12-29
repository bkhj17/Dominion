#pragma once
class BrickManager :
    public PoolingManager, 
    public Singleton<BrickManager>
{
private:
    const string BRICK_KEY = "Brick";
    const UINT MAX_HP = 5;

    friend class Singleton;
    BrickManager();
    ~BrickManager();

public:
    Direction Collision(GameObject* object);

    void LoadStage(string file);

private:

    // PoolingManager을(를) 통해 상속됨
    virtual void CreateObjects(string key, UINT poolSize) override;

    void ClearStage();
private:
    vector<vector<UINT>> stageData;

    UINT width = 0;
    UINT height = 0;

    HBRUSH* hBrushes;
};

