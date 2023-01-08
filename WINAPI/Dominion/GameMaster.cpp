#include "framework.h"
#include "DominionPlayer.h"
#include "CardDataManager.h"
#include "CardSupplier.h"
#include "Act.h"
#include "Card.h"
#include "CardSet.h"
#include "CardManager.h"
#include "GameMaster.h"
#include "SelectWindow.h"

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
	endButton->pos = { WIN_WIDTH - endButton->Half().x, 500.0f};
	endButton->isActive = false;

	trash = new CardSet(true, true);
	trash->isVisible = false;

	suppliers.resize(SUPPLIERS_NUM);
	for (auto& supplier : suppliers) {
		supplier = new CardSupplier();
		supplier->size = { 60.0f, 90.0f };
	}

	SelectWindow::Get()->SetResize({ CENTER_X, CENTER_Y }, { 800.0f, 400.0f });
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

	for (int i = 0; i < players.size(); i++) {
		for (int j = 0; j < 7; j++) {
			Card* copper = suppliers[0]->SupplyCard();
			players[i]->deck->InputCard(copper, false, true);
		}
		for (int j = 0; j < 3; j++) {
			Card* estate = suppliers[3]->SupplyCard();
			players[i]->deck->InputCard(estate, false, true);
		}
		players[i]->deck->Shuffle();

		for(int j = 0; j < 5; j++)
			players[i]->hand->InputCard(players[i]->deck->Pop());

		Observer::Get()->ExcuteParamEvents("CalcScore", players[i]);
	}

	
	turnPlayer = -1;
	NextTurn();

	auto testAct = new MainGameAct();

	mainAct = testAct;
	curAct = mainAct;
	curAct->Init();
}

void DominionGameMaster::Update()
{
	switch (state)
	{
	case DominionGameState::Start:
		state = DominionGameState::Playing;
		GameStart();
		break;
	case DominionGameState::Playing:
		if (!curAct->isDone) {
			curAct->Update();
		}
		else {
			//게임 종료
			state = DominionGameState::End;
			return;
		}

		if (control == turnPlayer)
			endButton->Update();
		break;
	case DominionGameState::End:
		break;
	}

	for (auto player : players) {
		player->Update();
	}
	CardManager::Get()->Update();
}

void DominionGameMaster::Render(HDC hdc)
{
	for (auto& supplier : suppliers) {
		supplier->Render(hdc);
	}

	players[control]->Render(hdc);
	players[side]->Render(hdc);

	if (!strExplain.empty()) {
		TextOutA(hdc, 
			(int)(endButton->Right() - strExplain.size() * 8), 
			(int)endButton->Top() - 20, 
			strExplain.c_str(), 
			(int)strExplain.size());
	}

	SelectWindow::Get()->Render(hdc);

	if (control == turnPlayer)
		endButton->Render(hdc);
}

bool DominionGameMaster::GameEndTrigger()
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
	players[0]->name = "사람";
	control = 0;
	//화면 안 보는 놈. AI로 돌릴꺼다
	players[1] = new DominionPlayer(false, true);
	players[1]->name = "봇";
	side = 1;

	Observer::Get()->AddParamEvent("CalcScore", [this](void* player) {
		auto p = (DominionPlayer*)player;
		if (p == nullptr)
			return;
		
		p->CalcScore();
	});
}

void DominionGameMaster::MakeSuppliers( )
{
	auto& datas = CardDataManager::Get()->datas;

	//재물, 기본 승점 공급처
	int defaultCardNum[] = { 60, 40, 30, 8, 8, 8, 10 };

	for (int i = 0; i < 3; i++) {
		auto& coin = suppliers[i];
		coin->Init(i, defaultCardNum[i]);
		coin->pos = { WIN_WIDTH * 0.2f + 260.0f, 200.0f+(2-i)*coin->size.y };
	}

	for (int i = 0; i < 2; i++) {
		auto& victory = suppliers[i+4];
		victory->Init(i+4, defaultCardNum[i+4]);
		victory->pos = { WIN_WIDTH * 0.2f + 200.0f, 200.0f + (1-i) * victory->size.y };
	}
	//사유지. 플레이어한테 나눠줄 것도 미리 만들자
	auto& estate = suppliers[(int)CardKey::ESTATE];
	estate->Init((int)CardKey::ESTATE, defaultCardNum[3]+ 3 * (int)players.size());
	estate->pos = { WIN_WIDTH * 0.2f + 200.0f, suppliers[4]->pos.y + estate->size.y};
	//저주. 사유지 아래
	auto& curse = suppliers[(int)CardKey::CURSE];
	curse->Init((int)CardKey::CURSE, defaultCardNum[3] + 3 * (int)players.size());
	curse->pos = { WIN_WIDTH * 0.2f + 200.0f, estate->pos.y + curse->size.y };

	const int KINGDOM_NUM = 10;
	auto kingdomKey = GetRandomSupplierKey(KINGDOM_NUM);
	int kingdomCol = 5;
	for (int i = 7; i < SUPPLIERS_NUM; i++) {
		int kingdomIndex = i - 7;

		int row = kingdomIndex / kingdomCol;
		int col = kingdomIndex % kingdomCol;

		auto& kingdom = suppliers[i];
		kingdom->size = { 80.0f, 120.0f };
		kingdom->pos = {
			WIN_WIDTH * 0.2f + 380.0f + col * kingdom->size.x,
			240.0f + row * kingdom->size.y
		};
		if(CardDataManager::Get()->datas[kingdomKey[kingdomIndex]].type[(int)CardType::VICTORY])
			kingdom->Init(kingdomKey[kingdomIndex], 8);
		else 
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

	
	vector<CardKey> setkey = {
		CardKey::SMITHY,
		CardKey::VILLAGE,
		CardKey::MARKET,
		CardKey::LABORATORY,
		CardKey::GARDENS,
		CardKey::FESTIVAL,
		CardKey::COUNCILROOM,
		CardKey::ARTISAN,
		CardKey::HARBINGER,
		CardKey::POACHER,
	};

	for (int i = 0; i < num; i++) {
		int p = Random(i, (int)v.size()-1);

		if (i < setkey.size()) {
			p = (int)setkey[i] - 7;
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
	if (!players[turnPlayer]->IsController()) {
		side = turnPlayer;
	}
}

void DominionGameMaster::SetEndButton(string buttonText, Event endEvent, string explain)
{
	endButton->isActive = true;
	endButton->SetText(buttonText);
	endButton->SetEvent(endEvent);
	strExplain = explain;
}

void DominionGameMaster::OffEndButton()
{
	endButton->isActive = false;
	endButton->SetText("");
	endButton->SetEvent(nullptr);
	strExplain = "";
}
