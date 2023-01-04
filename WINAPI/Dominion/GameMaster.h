#pragma once

class Act;
class DominionPlayer;
class CardSupplier;
class CardSet;

class DominionGameMaster : public Singleton<DominionGameMaster>
{
	friend class Act;
private:
	friend class Singleton;
	DominionGameMaster();
	~DominionGameMaster();

public:
	void GameStart();
	void Update();
	void Render(HDC hdc);
	
	bool IsGameEnd();

	void MakePlayers();
	void MakeSuppliers();
	vector<int> GetRandomSupplierKey(int num = 10);


	CardData* GetMouseOn();

	DominionPlayer* GetTurnPlayer() { return players[turnPlayer]; }
	DominionPlayer* GetControlPlayer() { return players[control]; }
	DominionPlayer* GetSidePlayer() { return players[side]; }

	void NextTurn();
public:
	const int COIN_NUM = 3;
	const int VICTORY_NUM = 4;
	const int KINGDOM_NUM = 10;
	const int SUPPLIERS_NUM = 17;
	
	int turnPlayer;
	int control, side;
	vector<DominionPlayer*> players;
	vector<CardSupplier*> suppliers;

	CardSet* trash;

	bool test = true;

	Button* endButton;
private:
	Act* mainAct = nullptr;
	Act* curAct = nullptr;
};

