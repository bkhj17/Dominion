#include "framework.h"
#include "DominionPlayer.h"
#include "CardDataManager.h"
#include "CardSupplier.h"
#include "Act.h"
#include "Card.h"
#include "CardDataManager.h"
#include "CardManager.h"
#include "GameMaster.h"

GameMaster::GameMaster()
{	


	CardDataManager::Get()->LoadData();

	players.resize(2);
	players[0] = new DominionPlayer();
	players[1] = new DominionPlayer();

	MakeSuppliers();
}

GameMaster::~GameMaster()
{
	for (auto player : players)
		delete player;
	players.clear();

	for (auto supplier : suppliers)
		delete supplier;
	suppliers.clear();

	if (mainAct != nullptr)
		delete mainAct;

	CardDataManager::Delete();
}

void GameMaster::Update()
{

	if (curAct != nullptr) {
		curAct->Update();
	}
	else if(test) {
		test = false;
		curAct = new TestAct;
	}

	CardManager::Get()->Update();


}

void GameMaster::Render(HDC hdc)
{
	for (auto& supplier : suppliers) {
		supplier->Render(hdc);
	}
}

bool GameMaster::IsGameEnd()
{
	int cnt = 0;
	for (auto supplier : suppliers) {
		if (supplier->Empty()) {
			cnt++;
			if (supplier->data->name == "╪саж")
				return true;
		}
	}
	return cnt >= 3;
}

void GameMaster::MakeSuppliers( )
{
	int numSuppliers = 3 + 4 + 10;
	suppliers.reserve(numSuppliers);

	auto& datas = CardDataManager::Get()->datas;
	for (int i = 0; i < 3; i++) {
		CardManager::Get()->CreateObjects(datas[i].key, 10);
		
		auto coin = new CardSupplier();
		

		
		coin->Init(&datas[i]);
		coin->size = { 60.0f, 90.0f };
		coin->pos = { WIN_WIDTH * 0.2f + 260.0f, 150.0f+i*90.0f };




		suppliers.push_back(coin);
	}
}

CardData* GameMaster::GetMouseOn()
{
	for (auto supplier : suppliers) {
		if (supplier->IsPointCollision(mousePos)) {
			return supplier->data;
		}
	}

	return nullptr;
}


