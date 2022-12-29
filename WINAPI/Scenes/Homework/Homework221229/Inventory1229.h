#pragma once
class BagItem1229 {
public:
	BagItem1229(ItemData1229* data);

	ItemData1229* data = nullptr;
};

class BagEquip1229 : public BagItem1229 {
public:
	BagEquip1229(ItemData1229* data);
};

class BagUsable1229 : public BagItem1229 {
public:
	BagUsable1229(ItemData1229* data, int count = 0);
	int count;
};

class Inventory1229 : public Singleton<Inventory1229>
{
private:
	friend class Singleton;	
	friend class SellScene1229;
	friend class InventoryScene1229;

	const int START_MONEY = 2000;

public:
	bool BuyItem(ItemData1229* data);

	void AddItem(ItemData1229* data);
	int HaveItem(int itemKey);

	bool UseMoney(int pay);
	void GainMoney(int gain);

	void SellItem(int index);
	int GetItemCount(int itemKey);
	
	
	int GetMoney() { return money; }
private:
	int money = START_MONEY;
	vector<BagItem1229*> items;
};

