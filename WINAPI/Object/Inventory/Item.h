#pragma once
class Item : public Button
{
public:
	Item(ItemData data);
	~Item();

	void Update();
	void Render(HDC hdc);

	Vector2& LocalPos() { return localPos; }
	ItemData& GetData() { return data; }

	void ClickStoreItem();
	void ClickInventoryItem();
private:
	Vector2 localPos;
	ItemData data;
};

