#pragma once

class HomeworkScene221221 :
    public Scene
{
    class Bar1221 : public Rect {
    public:
        Bar1221() = default;
        Bar1221(Vector2 pos, Vector2 size);
        ~Bar1221();

        Vector2 GetBarrelPos() const { return pos + barrelPos; }

        virtual void Update() override;
        virtual void Render(HDC hdc) override;

        bool isHoldBall = true;

        Vector2 GetBarrelDist() const { return (barrel->end - barrel->pos).GetNormalized(); }
    private:
        float speed = 500.0f;

        Line* barrel;
        Vector2 barrelPos;
        float barrelLength = 100.0f;
        float angle = 3.14f / 2;

        HBRUSH hBrush = nullptr;
    };

    class Ball1221 : public Circle {
    public:
        static const int SIZE = 30;

        Ball1221(const float size = SIZE);
        ~Ball1221();

        virtual void Update() override;

        virtual void Render(HDC hdc) override;

        void BoundToWar();
        bool CollisionToRect(Rect* lane);
    public:
        Vector2 distance;
        float speed = 300.0f;

    private:
        HBRUSH hBrush = nullptr;
    };


    class Block1221 : public Rect {
    public:
        static const float SIZE_X;
        static const float SIZE_Y;

        Block1221();
        ~Block1221();

        virtual void Render(HDC hdc) override;

        void Spawn(const Vector2& pos);
        void Destroy();
    private:

        HBRUSH hBrush = nullptr;
    };

    class BlockManager1221 : public PoolingManager, public Singleton<BlockManager1221>
    {
    private:
        friend class Singleton;
        BlockManager1221();

    public:
        void SpawnObject(const Vector2& pos);
        int Count();

        void Collision(Ball1221 * ball);
    private:
        // PoolingManager을(를) 통해 상속됨
        virtual void CreateObjects(string key, UINT poolSize) override;

    };

    enum class GameState {
        READY,
        PLAY,
        CLEAR,
        GAME_OVER
    };
    GameState gameState = GameState::READY;

public:
    HomeworkScene221221();
    ~HomeworkScene221221();
    // Scene을(를) 통해 상속됨
    virtual void Init() override;


    void UpdateReady();
    void UpdateGame();
    virtual void Update() override;

    virtual void Render(HDC hdc) override;
private:
    Bar1221* bar = nullptr;
    Ball1221* ball = nullptr;


};

