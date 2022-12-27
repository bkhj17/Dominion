#pragma once
class Fighter :
    public BattleCharacter
{
private:
    enum ActionType : int {
        IDLE, WALK, RUN, PUNCH, KICK, JUMP, DOUBLE_JUMP, JUMP_DOWN
    };

    const float WALK_SPEED = 100.0f;
    const float RUN_SPEED = 200.0f;
    const float RUN_TOUCH_DELAY = 0.5f;
public:
    Fighter();
    ~Fighter();

    virtual void Update() override;
    virtual void Render(HDC hdc) override;
private:
    void Control();
    void Attack();

    virtual void SetAnimation() override;
    virtual void CreateAnimations() override;

    void CheckMoveTouchTime();
private:
    bool isRun = false;
    bool isMoveTouch = false;
    float touchTime = 0.0f;
};

