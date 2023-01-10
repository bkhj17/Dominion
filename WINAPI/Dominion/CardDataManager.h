#pragma once
class CardData;

class CardDataManager : public Singleton<CardDataManager>
{
private:
	friend class Singleton;
	friend class DominionGameMaster;
	CardDataManager();

public:
	const CardData* GetData(int key);

	unordered_map<int, function<int(void*)>> sVictoryFunc;

	//카드 텍스처 : 하나로 묶여있음
	Texture* texture = nullptr;

	void RenderCovered(HDC hdc, Rect* cardRect);
	void RenderCovered(HDC hdc, Rect* cardRect, int alpha);
private:
	void LoadData();

	unordered_map<int, CardData> datas;
};

namespace SpecialVictory {
	int GardenVictory(void* player);
};