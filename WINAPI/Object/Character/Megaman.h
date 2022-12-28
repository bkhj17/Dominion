#pragma once
class Megaman :
    public ImageRect
{
private:
    enum class ActionType {
        IDLE, RUN, JUMP_UP, JUMP_DOWN
    };

    const float JUMP_POWER = 600.0f;
    const float GRAVITY = 980.0f;

public:
    Megaman();
    ~Megaman();

    void Update();
    void Render(HDC hdc);

    void SetLand(Texture* texture) { landTexture = texture; }

    Vector2 GetVelocity() { return velocity; }
    
    bool LandCollision(Rect* rect);
    Rect* GetBody() { return bodyRect; }
private:
    void Fire();

    void Move();
    void Jump();

    void SetAnimation();

    void SetAction(ActionType type);
    void CreateAnimations();

private:
    map<ActionType, Animation*> animations;

    ActionType curType = ActionType::IDLE;
    Vector2 velocity;

    float speed = 400.0f;

    int jumpCount = 0;

    Texture* landTexture;

    Rect* bodyRect;
    Vector2 bodyOffset;

    HBRUSH hBrush;
    HPEN hPen;

    float expRange = 200.0f;
};

