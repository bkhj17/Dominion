#pragma once

class CardData;

class InfoBox :
    public ImageRect
{
public:
    InfoBox();
    ~InfoBox();

    void Init();
    void Update();
    void Render(HDC hdc);

    void SetData(const CardData* data) { if(data != nullptr) dataInfo = data; }

    //위치, 사이즈 변경
    void SetPos(Vector2 pos);
    void SetPos(Vector2 pos, Vector2 size);
private:
    //텍스트 1줄 출력
    void RenderOneLine(HDC hdc, string str, Vector2 startPos, int& cnt);
    //카드 텍스트 출력
    void RenderText(HDC hdc);
    //플레이어 점수 출력
    void RenderScore(HDC hdc);
private:
    const int TEXT_SIZE = 15;

    //띄울 카드 데이터
    const CardData* dataInfo = nullptr;

    //카드 그림 띄울 위치
    const Vector2 IMAGE_POS = { 0.0f, 40.0f };
    const Vector2 IMAGE_SIZE = { 200.0f, 300.0f };
    //카드 그림 띄울 칸
    Rect* showImage;

    //점수 띄울 칸
    ImageRect* scoreBox;
};

