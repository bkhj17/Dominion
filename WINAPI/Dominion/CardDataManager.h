#pragma once
class CardData;

class CardDataManager : public Singleton<CardDataManager>
{
private:
	friend class Singleton;
	CardDataManager();

public:
	unordered_map<int, CardData> datas;
	Texture* texture = nullptr;

	void RenderCovered(HDC hdc, Rect* rect);
private:
	void LoadData();
};

