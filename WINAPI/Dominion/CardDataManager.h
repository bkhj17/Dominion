#pragma once
class CardData;

class CardDataManager : public Singleton<CardDataManager>
{
	friend class Singleton;
public:
	unordered_map<int, CardData> datas;
	Texture* texture = nullptr;

	void LoadData();

	int FindByName();

};

