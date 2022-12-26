#pragma once

class Player1226 : public Rect {
    enum class ActionType {
        IDLE, RUN, ATTACK, HIT, DEAD
    };
public:
    Player1226(bool isControl);
    ~Player1226();

    void SetEnemy(Player1226*);
    virtual void Update() override;
    virtual void Render(HDC hdc) override;

    void Input();
    void AI();
    void Move();
    void Attack();
    int AttackPoint() { return attack; }
    
    void Hit(int atk);
    void Dead();

    void SetAction(ActionType type);
    void SetAnimation();
    void CreateAnimations();

    bool IsHit() { return isHit; }
    bool IsDead() { return isDead; }

    bool IsAttack() { return isAttack; }
    Rect GetAttackRect();

    void SetDeadEvent(function<void ()> func);
private:
    void AttactEnd() {
        isAttack = false; 
        SetAction(ActionType::IDLE);
    }
    void HitEnd() { isHit = false; SetAction(ActionType::IDLE); }

    Texture* texture;
    ImageRect* imageRect;
    Vector2 ImageOffset;

    map<ActionType, Animation*> animations;
    ActionType curAction;

    int curHp, maxHp = 50.0f;
    int attack = 10.0f;
    float attackRange = 50.0f;

    float velocityX = 0.0f;
    float moveSpeed = 300.0f;

    bool isAttack = false;
    bool isDead = false;
    bool isHit = false;

    bool isControl = false;

    Player1226* enemy = nullptr;


    int alpha = 255;
};

class Homework221226 : public Scene
{
private:

public:
    Homework221226();
    ~Homework221226();

    // Scene을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;

    void GameEnd();
private:
    Player1226* player;
    Player1226* monster;

    ImageRect* bg;

    bool gameEnd = false;
};

