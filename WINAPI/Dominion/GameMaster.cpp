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
	MakePlayers();
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
		curAct = new BuyCardAct(nullptr, players[0]);
	}

	CardManager::Get()->Update();


}

void GameMaster::Render(HDC hdc)
{
	players[control]->Render(hdc);
	players[side]->Render(hdc);

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
			if (supplier->data->name == "속주")
				return true;
		}
	}
	return cnt >= 3;
}

void GameMaster::MakePlayers()
{
	players.resize(2);
	//화면 보는 놈
	players[0] = new DominionPlayer(true);
	control = 0;

	//화면 안 보는 놈. AI로 돌릴꺼다
	players[1] = new DominionPlayer(false);
	side = 1;
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
		coin->pos = { WIN_WIDTH * 0.2f + 260.0f, 200.0f+i*90.0f };
		suppliers.push_back(coin);
	}

	for (int i = 3; i < 7; i++) {
		CardManager::Get()->CreateObjects(datas[i].key, 10);
		auto victory = new CardSupplier();
		victory->Init(&datas[i]);
		victory->size = { 60.0f, 90.0f };
		victory->pos = { WIN_WIDTH * 0.2f + 200.0f, 200.0f + (i-3) * 90.0f };
		suppliers.push_back(victory);
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


