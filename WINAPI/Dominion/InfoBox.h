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

    //��ġ, ������ ����
    void SetPos(Vector2 pos);
    void SetPos(Vector2 pos, Vector2 size);
private:
    //�ؽ�Ʈ 1�� ���
    void RenderOneLine(HDC hdc, string str, Vector2 startPos, int& cnt);
    //ī�� �ؽ�Ʈ ���
    void RenderText(HDC hdc);
    //�÷��̾� ���� ���
    void RenderScore(HDC hdc);
private:
    const int TEXT_SIZE = 15;

    //��� ī�� ������
    const CardData* dataInfo = nullptr;

    //ī�� �׸� ��� ��ġ
    const Vector2 IMAGE_POS = { 0.0f, 40.0f };
    const Vector2 IMAGE_SIZE = { 200.0f, 300.0f };
    //ī�� �׸� ��� ĭ
    Rect* showImage;

    //���� ��� ĭ
    ImageRect* scoreBox;
};

