#pragma once

class CardData;

//카드 공급처 객체
class CardSupplier : public ImageRect
{
public:
	CardSupplier() = default;

	const CardData* data = nullptr;

	void Init(int key, int num = 10);
	void Render(HDC hdc);

	Card* SupplyCard();
	bool Empty() { return num <= 0; }

	void SetSelectable(function<bool(CardSupplier*)> condition);
	bool IsSelectable() { return selectable; }
private:
	int num = 0;

	bool selectable = false;
	HPEN selectablePen = nullptr;
};

