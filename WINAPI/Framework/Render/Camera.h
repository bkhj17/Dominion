#pragma once

class Rect;
class GameObject;

class Camera : public Singleton<Camera>
{
private:
    friend class Singleton;

    Camera();
    ~Camera();

public:
    void Update();

    void SetTarget(GameObject* target) { this->target = target; }
    void SetOffset(const float& x, const float& y) { offset = { x, y }; }
    void SetMapRect(Rect* rect) { mapRect = rect; }

    Vector2 GetPos();
    void SetPos(Vector2 pos);
private:
    void FreeMove();
    void FollowMode();

    void FixPos(Vector2& pos);

private:
    Rect* rect;
    float moveSpeed = 500.0f;
    float followSpeed = 5.0f;

    GameObject* target = nullptr;
    Vector2 offset;
    Vector2 destPos;

    Rect* mapRect;
};

