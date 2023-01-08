#pragma once
class Megaman :
    public PixelCharacter
{
private:
    enum ActionType : int {
        IDLE, RUN, JUMP_UP, JUMP_DOWN, PLANT
    };

public:
    Megaman();
    ~Megaman();

    virtual void Update() override;

    Vector2 GetVelocity() { return velocity; }
    
    bool LandCollision(Rect* cardRect);
    Rect* GetBody() { return bodyRect; }
private:
    void Fire();

    void Move();
    void Control();

    void SetAnimation();

    void SetIdle() { SetAction((int)ActionType::IDLE); }
    void CreateAnimations();

private:
    Rect* bodyRect;
    Vector2 bodyOffset;

    float expRange = 200.0f;
};

