#pragma once
#include "Scene.h"
class SpawnScene :
    public Scene
{
public:
    SpawnScene();
    ~SpawnScene();
    // Scene��(��) ���� ��ӵ�
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;

private:

};

