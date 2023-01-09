#pragma once

class InfoBox :
    public ImageRect
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

    void SetPos(Vector2 pos);

private:
    void RenderText(HDC hdc);
    void RenderOneLine(HDC hdc, string str, Vector2 startPos, int& cnt);
    void RenderScore(HDC hdc);
private:
    Vector2 imagePos;
    Rect* image;
    class CardData* dataInfo = nullptr;

    ImageRect* scoreBox;

};

