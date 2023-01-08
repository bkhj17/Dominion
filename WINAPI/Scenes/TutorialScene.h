#pragma once

class TutorialScene :
    public Scene
{
public:
    TutorialScene();
    ~TutorialScene();

    // Scene을(를) 통해 상속됨
    virtual void Start() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;

private:
    //POINT rectPos = { (int)CENTER_X, (int)CENTER_Y };
    Rect* cardRect;
    int speed = 10;

    HBRUSH hBrush;
};

