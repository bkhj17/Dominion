#pragma once

class BombManager :
    public PoolingManager, public Singleton< BombManager>
{
private:
    friend class Singleton;
    const UINT POOL_SIZE = 50;
    BombManager();
    ~BombManager();
public:
    void Plant(const Vector2 pos);
    void CreateObjects(string key, UINT poolSize);

    void SetLand(Texture* texture);

    void Clear();
private:
    HBRUSH hBrush;
    HPEN hPen;
};

