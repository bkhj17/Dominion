#pragma once
class CardData;

class CardDataManager : public Singleton<CardDataManager>
{
private:
	friend class Singleton;
	CardDataManager();

public:
	unordered_map<int, CardData> datas;
	unordered_map<int, function<int(void*)>> sVictoryFunc;

	Texture* texture = nullptr;

	void RenderCovered(HDC hdc, Rect* cardRect);
	void RenderCovered(HDC hdc, Rect* cardRect, int alpha);
private:
	void LoadData();
};

namespace SpecialVictory {
	int GardenVictory(void* player);
};