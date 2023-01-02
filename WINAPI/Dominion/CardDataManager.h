#pragma once
class CardData;

class CardDataManager : public Singleton<CardDataManager>
{
	friend class Singleton;
public:
	unordered_map<int, CardData> datas;
	Texture* texture;

	void LoadData();

	int FindByName();

};

