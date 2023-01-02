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
	void Update();
	void Render(HDC hdc);

	bool IsGameEnd();

	void MakeSuppliers();

	CardData* GetMouseOn();

public:
	vector<DominionPlayer*> players;
	vector<CardSupplier*> suppliers;

	bool test = true;
	Act* curAct = nullptr;
	int curPlayer = 0;
private:
	Act* mainAct = nullptr;
};

