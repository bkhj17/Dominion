#pragma once

class GameObject;

class CollisionScene :
    public Scene
{
public:
    CollisionScene();
    ~CollisionScene();
    // Scene��(��) ���� ��ӵ�
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;

private:
    vector<GameObject*> objects;

    HBRUSH hRedBrush;
    HBRUSH hBlueBrush;

    bool isCollision = false;
};

