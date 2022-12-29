#pragma once
#include "Scene.h"
class AngleScene :
    public Scene
{
public:
    AngleScene();
    ~AngleScene();

    // Scene��(��) ���� ��ӵ�
    virtual void Start() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;
private:
    Cannon* cannon;
};

