#include "framework.h"

DataManager1229::DataManager1229()
{
	LoadItemTable();
}

DataManager1229::~DataManager1229()
{
	for (auto data : itemTable)
		delete data.second;
	itemTable.clear();
}

ItemData1229* DataManager1229::GetItemData(int itemKey)
{
	if (itemTable.count(itemKey) == 0)
		return nullptr;

	return itemTable[itemKey];
}

void DataManager1229::LoadItemTable()
{
	ifstream ifs;
	ifs.open(FILE_ITEM_TABLE);
	if (ifs.fail()) {
		cout << "아이템 테이블 로드 실패.\n";
		return;
	}

	while (!ifs.eof()) {
		string s;
		ifs >> s;
		if (s.empty())
			return;

		auto vs = SplitString(s, ",");

		int key = stoi(vs[0]);
		int category = stoi(vs[1]);

		ItemData1229* data;
		if (category == 0) {
			data = new ItemData1229();
			data->key = key;
			data->category = category;
			data->name = vs[2];
			data->price = stoi(vs[3]);
			itemTable[data->key] = data;
		} else if (category == 1) {
			data = new EquipItemData1229();
			data->key = key;
			data->category = category;
			data->name = vs[2];
			data->price = stoi(vs[3]);
			for (int i = 4; i < vs.size(); i += 2)
				((EquipItemData1229*)data)->forces.push_back({ vs[i], stoi(vs[i + 1]) });
			itemTable[data->key] = data;
		}

	}
	ifs.close();
}
