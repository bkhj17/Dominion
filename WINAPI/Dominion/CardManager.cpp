#include "framework.h"
#include "CardManager.h"
#include "Card.h"
#include "CardDataManager.h"
#include "GameMaster.h"
#include "MyMovement.h"

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

	auto& data = CardDataManager::Get()->datas[key];

	if (totalObjects.find(data.name) != totalObjects.end()) {
		//�̹� ��������� ���� ���
		totalObjects[data.name].reserve(poolSize);
		while (poolSize > totalObjects[data.name].size()) {
			Card* card = new Card(&data);
			card->isActive = false;
			totalObjects[data.name].push_back(card);
		}
	}
	else {
		//���� ������ �ϴ� ���
		vector<GameObject*> cards(poolSize);
		for (auto& card : cards) {
			card = new Card(&data);
			card->isActive = false;
		}
		totalObjects[data.name] = cards;
	}
	//ī�� Ű�� ���� ī�� ����
}

GameObject* CardManager::PopByIntKey(int key)
{
	auto card = (Card*)Pop(CardDataManager::Get()->datas[key].name);
	card->selectablePen = selectablePen;
	card->selectedPen = selectedPen;
	return card;
}
