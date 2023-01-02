#include "framework.h"
#include "CardManager.h"
#include "Card.h"
#include "CardDataManager.h"

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
	return Pop(CardDataManager::Get()->datas[key].name);
}
