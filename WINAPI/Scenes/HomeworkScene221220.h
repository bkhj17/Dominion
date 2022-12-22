#pragma once
#include "Scene.h"
class HomeworkScene221220 :
    public Scene
{
    class Plane1220 : public Rect {
        static const Vector2 DEFAULT_SIZE;
    public:
        Plane1220(Vector2 pos = { 0,0 });

        void Update();
        virtual void Render(HDC hdc) override;

        bool Shot();

        Vector2 ShotPos() { return pos + shotPos; }

    private:
        void Move();

        Vector2 shotPos;

        float moveSpeed = 1000.0f;

        float shotWait = 0.2f;
        float shotRate = 0.2f;

        Rect wing;
        Rect body;
    };

    class Bullet1220 : public Circle {
        static const float DEFUALT_RADIUS;
        static const float DEFUALT_SPEED;
    public:
        Bullet1220();
        void Init(Vector2 pos);
        void Update();
        void Destroy();
    private:
        float speed = DEFUALT_SPEED;
    };

    class Monster : public Rect {
        static const float MONSTER_SIZE_MIN;
        static const float MONSTER_SIZE_MAX;
    public:
        Monster();

        void Init();
        void Update();

        void Destroy();
    private:
        float speed = 1000.0f;
    };

public:
    HomeworkScene221220();
    ~HomeworkScene221220();

    // Scene을(를) 통해 상속됨
    virtual void Init() override;

    virtual void Update() override;

    virtual void Render(HDC hdc) override;

private:
    void ShotBullet();
    void UpdateBullets();
    void SpawnMonsters();
    void UpdateMonsters();

    Plane1220* plane = nullptr;

    vector<Bullet1220*> bullets;
    vector<Monster*> monsters;

    HBRUSH hPlaneBrush = nullptr;
    HBRUSH hBulletBrush = nullptr;
    HBRUSH hMonsterBrush = nullptr;

    float monsterWait = 0.0f;
    float monsterRate = 1.0f;

    bool isPlaying = true;

    int score = 0;
};

