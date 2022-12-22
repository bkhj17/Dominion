#pragma once
#include "Scene.h"
class BrickOutScene : public Scene
{
public:
    BrickOutScene();
    ~BrickOutScene();


    // Scene��(��) ���� ��ӵ�
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;

private:
    ControlBar* controlBar = nullptr;
    Ball* ball = nullptr;
};

