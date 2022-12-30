#pragma once
#include "Panel.h"
class Inventory :
    public Panel
{
public:
    Inventory();
    ~Inventory();

    void Update() override;
    void Render(HDC hdc) override;

    void PurchaseItem(void* item);
    void SellItem();
private:
    void SetItemPos(int i);
    void SetItemPos();
private:
    int gold = 100000;
    Vector2 goldPos;
};

