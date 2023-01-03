#pragma once

class Act;
class DominionPlayer;
class CardSupplier;

class GameMaster : public Singleton<GameMaster>
{
	friend class Act;
private:
	friend class Singleton;
	GameMaster();
	~GameMaster();

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
	
public:
	int turnPlayer;
	int control, side;
	vector<DominionPlayer*> players;
	vector<CardSupplier*> suppliers;

	bool test = true;

	Button* endButton;
private:
	Act* mainAct = nullptr;
	Act* curAct = nullptr;
};

