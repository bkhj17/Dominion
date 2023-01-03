#pragma once

class CardData;

class CardSupplier : public ImageRect
{
public:
	CardSupplier();

	CardData* data = nullptr;

	void Init(int key, int num = 10);
	void Render(HDC hdc);

	Card* SupplyCard();
	bool Empty() { return num <= 0; }

private:
	int num = 0;
};

