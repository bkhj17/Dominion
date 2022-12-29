#pragma once
class Bomb :
    public ImageRect
{
private: 
    const float EXP_TIME = 3.0f;
    const float EXP_RANGE = 100.0f;

    const float GRAVITY = 980.0f;
public:
    Bomb();
    ~Bomb() = default;

    virtual void Update() override;
    virtual void Render(HDC hdc) override;

    void Plant(const Vector2& pos);

    void SetLand(Texture* texture) { landTexture = texture; }

    void SetBrush(HBRUSH brush, HPEN pen) { hBrush = brush, hPen = pen; }
private:
    void Fall();
    void Timer();

private:
    float time = 0.0f;
    float velocity = 0.0f; //상하 속도

    Texture* landTexture;

    HBRUSH hBrush;
    HPEN hPen;
};

