#include "framework.h"
#include "CardManager.h"
#include "Card.h"
#include "CardDataManager.h"
#include "GameMaster.h"

CardManager::CardManager()
{
	selectablePen = CreatePen(PS_SOLID, 3, GREEN);
	selectedPen = CreatePen(PS_SOLID, 3, YELLOW);
}

CardManager::~CardManager()
{
	DeleteObject(selectablePen);
	DeleteObject(selectedPen);
}

void CardManager::CreateObjects(int key, UINT poolSize)
{
	vector<GameObject*> cards(poolSize);
	auto& data = CardDataManager::Get()->datas[key];
	for (auto& card : cards) {
		card = new Card(data);
	}
	totalObjects[data.name] = cards;
	//ī�� Ű�� ���� ī�� ����
}

GameObject* CardManager::PopByIntKey(int key)
{
	auto card = (Card*)Pop(CardDataManager::Get()->datas[key].name);
	card->selectablePen = selectablePen;
	card->selectedPen = selectedPen;
	return card;
}
