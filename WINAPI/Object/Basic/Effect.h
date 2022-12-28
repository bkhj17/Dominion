#pragma once
class Effect :
    public ImageRect
{
public:
    Effect(wstring file, UINT frameX, UINT frameY, float speed = 3.0f, bool loop = false, COLORREF trans = MAGENTA);
    Effect(Texture* texture, float speed = 3.0f, bool loop = false);
    
    ~Effect();

    void Update();
    void Render(HDC hdc);

    void Play(const Vector2& pos);
    void Stop();
private:
    Animation* animation;
};

