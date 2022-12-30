#pragma once
class Panel :
    public ImageRect
{
public:
    Panel(wstring file);
    ~Panel();

    virtual void Update() override;
    virtual void Render(HDC hdc) override;

    void SelectItem(void* item);

protected:
    void Drag();

protected:
    Vector2 offset;
    bool isDrag = false;

    vector<Item*> items;

    // 원래 같으면 안 되는 짓거리
    ImageRect* thumbnailImage = nullptr;
    Vector2 thumbnailOffset = { -210.0f, -85.0f };
    Item* selectItem;

    string descStr;
};

