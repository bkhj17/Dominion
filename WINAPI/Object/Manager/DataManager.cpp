#include "framework.h"
#include "DataManager.h"

DataManager::DataManager()
{
	LoadItemTable();
}

DataManager::~DataManager()
{
}

void DataManager::LoadItemTable()
{
    ifstream loadFile;
    loadFile.open("TextData/Inventory/ItemTable1.csv");

    bool isFirst = true;

    string temp;
    while (1) {
        temp = "";
        loadFile >> temp;

        if (isFirst) {
            isFirst = false;
            continue;
        }

        vector<string> datas = SplitString(temp, ",");
        if (datas.empty())
            break;

        ItemData data;
        data.key = stoi(datas[0]);
        data.name = datas[1];
        data.price = stoi(datas[2]);
        data.file.assign(ToWString("Textures/UI/Item/" + datas[3]));
        
        itemDatas[data.key] = data;

        if (loadFile.eof())
            break;
    }
    loadFile.close();
}
