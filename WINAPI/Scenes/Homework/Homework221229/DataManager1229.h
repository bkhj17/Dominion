#pragma once
class ItemData1229 {
public:
	int key = -1;
	int category = 0;
	int price = 0;
	string name = "";
};

class EquipItemData1229 : public ItemData1229 {
public:
	vector<pair<string, int>> forces;
};

class DataManager1229 : public Singleton<DataManager1229>
{
	const string FILE_ITEM_TABLE = "TextData/Inventory/ItemTable.csv";
private:
	friend class Singleton;
	DataManager1229();
	~DataManager1229();

public:

	ItemData1229* GetItemData(int itemKey = 0);
	
	void LoadItemTable();

	const map<int, ItemData1229*>& ItemTable() { return itemTable; }
private:
	map<int, ItemData1229*> itemTable;
};

