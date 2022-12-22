#pragma once
class Cannon :
    public Circle
{
public:
    Cannon();
    ~Cannon();

    void Update();
    void Render(HDC hdc);

private:
    Line* barrel = nullptr;

    float angle = 0.0f;
    float barrelLength = 200.0f;
};

