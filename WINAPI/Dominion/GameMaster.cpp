#include "framework.h"
#include "DominionPlayer.h"
#include "CardDataManager.h"
#include "CardSupplier.h"
#include "Act.h"
#include "Card.h"
#include "CardSet.h"
#include "CardDataManager.h"
#include "CardManager.h"
#include "GameMaster.h"

DominionGameMaster::DominionGameMaster()
{	
	MakePlayers();

	Texture* normal = Texture::Add(L"Textures/UI/NormalButton.bmp");
	Texture* over = Texture::Add(L"Textures/UI/OverButton.bmp");
	Texture* down = Texture::Add(L"Textures/UI/DownButton.bmp");

	endButton = new Button(normal);
	endButton->SetOverTexture(over);
	endButton->SetDownTexture(down);
	endButton->size = { 150.0f, 50.0f };
	endButton->pos = { WIN_WIDTH - endButton->size.x, 500.0f };
	endButton->isActive = false;

	trash = new CardSet(true, true);
	trash->isVisible = false;

	suppliers.resize(SUPPLIERS_NUM);
	for (auto& supplier : suppliers) {
		supplier = new CardSupplier();
		supplier->size = { 60.0f, 90.0f };
	}
}

DominionGameMaster::~DominionGameMaster()
{
	for (auto player : players)
		delete player;
	players.clear();

	for (auto supplier : suppliers)
		delete supplier;
	suppliers.clear();

	if (mainAct != nullptr)
		delete mainAct;

	delete trash;
	delete endButton;

}

void DominionGameMaster::GameStart()
{
	MakeSuppliers();

	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 2; j++) {
			Card* testCard = suppliers[0]->SupplyCard();
			players[j]->deck->InputCard(testCard, false, true);
		}
	}

	auto testCard = suppliers[7]->SupplyCard();
	players[0]->hand->InputCard(testCard, false, true);

	turnPlayer = -1;
	NextTurn();

	auto testAct = new MainGameAct();

	mainAct = testAct;
	curAct = mainAct;
	curAct->Init();
}

void DominionGameMaster::Update()
{
	if (test) {
		test = false;
		GameStart();
	}
	if (!curAct->isDone) {
		curAct->Update();
	}
	else {
		//게임 종료
	}

	CardManager::Get()->Update();

	if (control == turnPlayer)
		endButton->Update();
}

void DominionGameMaster::Render(HDC hdc)
{
	players[control]->Render(hdc);
	players[side]->Render(hdc);

	for (auto& supplier : suppliers) {
		supplier->Render(hdc);
	}

	if(control == turnPlayer)
		endButton->Render(hdc);
}

bool DominionGameMaster::IsGameEnd()
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

void DominionGameMaster::MakePlayers()
{
	players.resize(2);
	//화면 보는 놈
	players[0] = new DominionPlayer(true, false);
	control = 0;

	//화면 안 보는 놈. AI로 돌릴꺼다
	players[1] = new DominionPlayer(false, true);
	side = 1;
}

void DominionGameMaster::MakeSuppliers( )
{
	auto& datas = CardDataManager::Get()->datas;

	//재물, 기본 승점 공급처
	int defaultCardNum[] = { 60, 40, 30, 8, 8, 8, 10 };

	for (int i = 0; i < 3; i++) {
		auto& coin = suppliers[i];
		coin->Init(i, defaultCardNum[i]);
		coin->pos = { WIN_WIDTH * 0.2f + 260.0f, 200.0f+i*90.0f };
	}

	for (int i = 3; i < 7; i++) {
		auto& victory = suppliers[i];
		victory->Init(i, defaultCardNum[i]);
		victory->pos = { WIN_WIDTH * 0.2f + 200.0f, 200.0f + (i-3) * 90.0f };
	}

	const int KINGDOM_NUM = 10;
	auto kingdomKey = GetRandomSupplierKey(KINGDOM_NUM);
	int kingdomCol = 5;
	for (int i = 7; i < SUPPLIERS_NUM; i++) {
		int kingdomIndex = i - 7;

		int row = kingdomIndex / kingdomCol;
		int col = kingdomIndex % kingdomCol;

		auto& kingdom = suppliers[i];
		kingdom->pos = {
			WIN_WIDTH * 0.2f + 380.0f + col * kingdom->size.x,
			240.0f + row * kingdom->size.y
		};

		kingdom->Init(kingdomKey[kingdomIndex], 10);
	}
}

vector<int> DominionGameMaster::GetRandomSupplierKey(int num)
{
	//0부터 6은 기본 재물, 승점, 저주
	vector<int> v(CardDataManager::Get()->datas.size() - 7);
	for (int i = 7; i < CardDataManager::Get()->datas.size(); i++) {
		v[i-7] = i;
	}

	for (int i = 0; i < num; i++) {
		int p = Random(i, (int)v.size()-1);
		if (i == 0) {
			p = (int)CardKey::SMITHY-7;
		}
		swap(v[i], v[p]);
	}

	return v;
}

CardData* DominionGameMaster::GetMouseOn()
{
	for (auto supplier : suppliers) {
		if (supplier->IsPointCollision(mousePos)) {
			return supplier->data;
		}
	}

	Card* mouseOn = nullptr;
	for (auto player : players) {
		//패 정보
		mouseOn = player->hand->GetByPos(mousePos);

		if (mouseOn != nullptr) {
			if (!mouseOn->isActive || !mouseOn->isVisible || mouseOn->isCovered)
				mouseOn = nullptr;
			if (mouseOn != nullptr)
				return mouseOn->data;
		}
		//사용한 카드 정보
		mouseOn = player->used->GetByPos(mousePos);

		if (mouseOn != nullptr) {
			if (!mouseOn->isActive || !mouseOn->isVisible || mouseOn->isCovered)
				mouseOn = nullptr;
			if (mouseOn != nullptr)
				return mouseOn->data;
		}

	}

	return nullptr;
}

void DominionGameMaster::NextTurn()
{
	turnPlayer = (turnPlayer+1) % players.size();
	if (!players[turnPlayer]->isController) {
		side = turnPlayer;
	}
}
