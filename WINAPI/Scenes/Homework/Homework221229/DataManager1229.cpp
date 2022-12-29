#include "framework.h"

DataManager1229::DataManager1229()
{
	LoadItemTable();
}

DataManager1229::~DataManager1229()
{
}

ItemData1229* DataManager1229::GetItemData(int itemKey)
{
	if (itemTable.count(itemKey) == 0)
		return nullptr;

	return &itemTable[itemKey];
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

		ItemData1229 data;
		data.key = stoi(vs[0]);
		data.name = vs[1];
		data.price = stoi(vs[2]);
		itemTable[data.key] = data;
	}
	ifs.close();
}
