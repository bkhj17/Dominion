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

	delete endButton;
}

void GameMaster::GameStart()
{
	MakeSuppliers();

	turnPlayer = 0;
	players[turnPlayer]->TurnStart();
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 2; j++) {
			Card* testCard = suppliers[0]->SupplyCard();
			testCard->pos = players[j]->deckRect->pos;
			testCard->isVisible = false;
			players[j]->deck.push_back(testCard);
		}
	}

	auto testCard = suppliers[7]->SupplyCard();
	testCard->pos = players[0]->handRect->pos;
	testCard->size = { 60.0f, 90.0f };
	testCard->isVisible = true;
	players[0]->hand.push_back(testCard);

	auto testAct = new ActionPhaseAct(nullptr, players[turnPlayer]);

	mainAct = testAct;
	curAct = mainAct;
	curAct->Init();
}

void GameMaster::Update()
{
	if (test) {
		test = false;
		GameStart();
	}
	if (!mainAct->isDone) {
		mainAct->Update();
	}
	else {
		//
	}

	CardManager::Get()->Update();

	endButton->Update();
}

void GameMaster::Render(HDC hdc)
{
	players[control]->Render(hdc);
	players[side]->Render(hdc);

	for (auto& supplier : suppliers) {
		supplier->Render(hdc);
	}

	endButton->Render(hdc);
}

bool GameMaster::IsGameEnd()
{
	int cnt = 0;
	for (auto supplier : suppliers) {
		if (supplier->Empty()) {
			cnt++;
			if (supplier->data->name == "����")
				return true;
		}
	}
	return cnt >= 3;
}

void GameMaster::MakePlayers()
{
	players.resize(2);
	//ȭ�� ���� ��
	players[0] = new DominionPlayer(true, false);
	control = 0;

	//ȭ�� �� ���� ��. AI�� ��������
	players[1] = new DominionPlayer(false, true);
	side = 1;
}

void GameMaster::MakeSuppliers( )
{
	int numSuppliers = 3 + 4 + 10;
	suppliers.reserve(numSuppliers);

	auto& datas = CardDataManager::Get()->datas;

	//�繰, �⺻ ���� ����ó
	int defaultCardNum[] = { 60, 40, 30, 8, 8, 8, 10 };

	for (int i = 0; i < 3; i++) {
		auto coin = new CardSupplier();
		coin->Init(i, defaultCardNum[i]);
		coin->size = { 60.0f, 90.0f };
		coin->pos = { WIN_WIDTH * 0.2f + 260.0f, 200.0f+i*90.0f };
		suppliers.push_back(coin);
	}

	for (int i = 3; i < 7; i++) {
		auto victory = new CardSupplier();
		victory->Init(i, defaultCardNum[i]);
		victory->size = { 60.0f, 90.0f };
		victory->pos = { WIN_WIDTH * 0.2f + 200.0f, 200.0f + (i-3) * 90.0f };
		suppliers.push_back(victory);
	}

	const int KINGDOM_NUM = 10;
	auto kingdomKey = GetRandomSupplierKey(KINGDOM_NUM);
	int kingdomCol = 5;
	for (int i = 0; i < KINGDOM_NUM; i++) {
		int row = i / kingdomCol;
		int col = i % kingdomCol;

		auto kingdom = new CardSupplier();
		kingdom->Init(kingdomKey[i], 10);
		kingdom->size = { 60.0f, 90.0f };
		kingdom->pos = {
			WIN_WIDTH * 0.2f + 380.0f + col * kingdom->size.x,
			240.0f + row * kingdom->size.y
		};
		suppliers.push_back(kingdom);
	}
}

vector<int> GameMaster::GetRandomSupplierKey(int num)
{
	//0���� 6�� �⺻ �繰, ����, ����
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

CardData* GameMaster::GetMouseOn()
{
	for (auto supplier : suppliers) {
		if (supplier->IsPointCollision(mousePos)) {
			return supplier->data;
		}
	}

	for (auto card : players[turnPlayer]->hand) {
		if (card->IsPointCollision(mousePos)) {
			return card->data;
		}
	}

	return nullptr;
}
