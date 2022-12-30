#pragma once

class GameObject;

class CollisionScene :
    public Scene
{
public:
    CollisionScene();
    ~CollisionScene();
    // Scene을(를) 통해 상속됨
    virtual void Start() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;

private:
    vector<GameObject*> objects;

    HBRUSH hRedBrush;
    HBRUSH hBlueBrush;

    HPEN hRedPen;
    HPEN hBluePen;

    bool isCollision = false;
};

