#pragma once
#include "Scene.h"
class SpawnScene :
    public Scene
{
public:
    SpawnScene();
    ~SpawnScene();
    // Scene을(를) 통해 상속됨
    virtual void Start() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;

private:

};

