#pragma once

class CardData;

//ī�� ����ó ��ü
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

