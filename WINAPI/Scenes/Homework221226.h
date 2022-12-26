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

    void UpdateHPBar();
    void RenderHPBar(HDC hdc);

    void SetPos(Vector2 pos);
private:
    void AttactEnd() {
        isAttack = false; 
        SetAnimation();
    }
    void HitEnd() { isHit = false; SetAnimation(); }

    Texture* texture;

    ImageRect* imageRect;
    Vector2 imageOffset;

    Rect* hpbar;
    Vector2 hpOffset;

    map<ActionType, Animation*> animations;
    ActionType curAction;

    int curHp, maxHp = 50.0f;
    float maxInverse;
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

    HBRUSH* hBrush;
};

class MonsterManager : public PoolingManager, public Singleton<MonsterManager> {
    friend class Singleton;
    MonsterManager();

public:
    int GetCount();
    void SetEnemy(Player1226* player);
    vector<GameObject*>& GetMonsters() { return totalObjects["Monster"]; }

    void Collision(Rect* rect);
private:
    virtual void CreateObjects(string key, UINT poolSize);
};

class AttackBox1226 : public Rect, public Singleton<AttackBox1226>{
    friend class Singleton;
    AttackBox1226();
    ~AttackBox1226();
public:
    void Update();
    void Render(HDC hdc);

    void Collision();

    int attack = 0;
private:
    Animation* animation;
    ImageRect* image;

    float speed = 2000.0f;
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

    ImageRect* bg;
    bool gameEnd = false;
};

