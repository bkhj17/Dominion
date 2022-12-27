#pragma once
class Character : public Rect{
protected:
    const float JUMP_POWER = 700.0f;
    const float GRAVITY = 1500.0f;
    const float BOTTOM_CHECK = 10.0f;

    const float GHOST_TIME = 5.0f;
    const float ALPHA_SPEED = 100.0f;
public:
    Character(wstring file, int frameX, int frameY, COLORREF trans = MAGENTA);
    ~Character();

    virtual void Update();
    virtual void Render(HDC hdc, int alpha = 255);

protected:
    void SetAction(int type);

protected:
    Texture* texture;
    ImageRect* imageRect;
    Vector2 imageOffset;

    map<int, Animation*> animations;
    int curType = 0;

    float velocity = 0.0f;

    bool isGhost = false;
    float ghostTime = 0.0f;

};