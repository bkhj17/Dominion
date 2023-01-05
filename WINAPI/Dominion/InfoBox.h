#pragma once

class InfoBox :
    public Rect
{
public:
    InfoBox();
    ~InfoBox();

    void Init();
    void Update();
    void Render(HDC hdc);

    void SetData(CardData* data) { 
        if(data != nullptr)
            dataInfo = data; 
    }

private:
    void RenderText(HDC hdc, string str, Vector2 startPos, int& cnt);
private:
    Vector2 imagePos;
    Rect* image;
    class CardData* dataInfo = nullptr;
};

