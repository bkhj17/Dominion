#pragma once
class PixelCharacter : public ImageRect {

protected:
    const float JUMP_POWER = 600.0f;
    const float GRAVITY = 980.0f;
    const float CHECK_DIST = 20.0f;
    const float MAX_SLOPE = 30.0f;

public:
    PixelCharacter(wstring file, int frameX, int frmaeY);
    ~PixelCharacter();

    virtual void Update();
    virtual void Render(HDC hdc);

    void SetLand(Texture* texture) { landTexture = texture; }
protected:
    bool CanMove();
    virtual void Jump();

    void SetAction(int type);
    void SetIdle() { SetAction(0); }

    map<int, Animation*> animations;
    int curType = 0;

    Vector2 velocity;

    float speed = 400.0f;

    int jumpCount = 0;

    Texture* landTexture = nullptr;
};