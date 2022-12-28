#pragma once
class Character1228 :
    public ImageRect
{
protected:
    enum class ActionType {
        IDLE, RUN, JUMP_UP, JUMP_DOWN
    };

    const float JUMP_POWER = 600.0f;
    const float GRAVITY = 980.0f;

public:
    Character1228(wstring file, 
		UINT frameX = 1, 
		UINT frameY = 1, 
		COLORREF transColor = MAGENTA);
    ~Character1228();

    virtual void Update();
    void Render(HDC hdc);

    void SetLand(Texture* texture) { landTexture = texture; }

    Vector2 GetVelocity() { return velocity; }

    bool LandCollisionX();
    bool LandCollisionY();
    Rect* GetBody() { return bodyRect; }
protected:

    void Move();
    void Jump();

    void SetAnimation();

    void SetAction(ActionType type);
    virtual void CreateAnimations() = 0;

protected:
    map<ActionType, Animation*> animations;

    ActionType curType = ActionType::IDLE;
    Vector2 velocity;

    bool xCollision = false;

    float speed = 400.0f;

    int jumpCount = 0;

    Texture* landTexture = nullptr;

    Rect* bodyRect;
    Vector2 bodyOffset = { 0,0 };
};


class Bomb1228 : public Character1228 {
public:
    Bomb1228();
    ~Bomb1228();

    virtual void Update() override;

    void Spawn(Vector2 pos);
    void SetBoom(function<void(float, float)>& func) { boomFunc = func; }
private:

    virtual void CreateAnimations();

    const float LIFETIME = 3.0f;
    float lifeTime = 0.0f;

    function<void(float, float)> boomFunc;
};

class BombManager1228 : public PoolingManager, public Singleton<BombManager1228>
{
private:
    static const string KEY;

    friend class Singleton;
    BombManager1228();
    ~BombManager1228() = default;
public:
    void SetFunc(function<void(float, float)>& func);
    void Spawn(Vector2 pos);
    void SetLand(Texture* texture);
private:
    void CreateObjects(string key, UINT poolSize);
    function<void(float, float)> boomFunc;
};

class Player1228 : public Character1228 {
public:
    Player1228();
    ~Player1228();

    virtual void Update() override;
private:
    virtual void CreateAnimations() override;

    void Control();
};

class Monster1228 : public Character1228 {
public:
    Monster1228();
    ~Monster1228();

    virtual void Update() override;
private:
    virtual void CreateAnimations() override;

    void Control();

    float dest = 1.0f;
};

class Homework221228 :
    public Scene
{
public:
    Homework221228();
    ~Homework221228();

    // Scene을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;

    void Boom(float x, float y);
private:
    ImageRect* land;
    Player1228* player;
    Monster1228* monster;

    float expRange = 200.0f;

    HBRUSH hBrush;
    HPEN hPen;

    ImageRect* goal;
};

