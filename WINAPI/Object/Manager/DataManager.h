#pragma once

struct ItemData {
	int key;
	string name;
	int price;
	wstring file;
};

class DataManager : public Singleton<DataManager>
{
private:
	friend class Singleton;
	DataManager();
	~DataManager();

	void LoadItemTable();
public:
	map<int, ItemData> GetItemDatas() { return itemDatas; }

private:

	map<int, ItemData> itemDatas;
};

