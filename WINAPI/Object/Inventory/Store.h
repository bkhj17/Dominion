#pragma once
class Store :
    public Panel
{
public:
    Store();
    ~Store();

    void Update();
    void Render(HDC hdc);

private:
    void CreateItems();
    void CreateButtons();

    void OnClickPurchaseButton();
    void OnClickSellButton();
private:
    Button* purchaseButton;
    Vector2 purchaseOffset = { 230, -110 };

    Button* sellButton;
    Vector2 sellOffset = { 230, -60 };
};

