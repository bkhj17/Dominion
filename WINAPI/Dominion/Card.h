#pragma once

enum class CardKey : int {
	COOPER = 0,
	SILVER,
	GOLD,
	ESTATE,
	DUCHY,
	PROVINCE,
	CURSE,
	REMODEL,
	BUREAUCRAT,
	MINE,
	MONEYLENDER,
	SMITHY,
	LIBRARY,
	WITCH,
	VILLAGE,
	MILITIA,
	MARKET,
	LABORATORY,
	THRONE_ROOM,
	CHAPEL,
	CELLAR,
	GARDENS,
	WORKSHOP,
	FESTIVAL,
	MOAT,
	COUNCILROOM,
	ARTISAN,
	BANDIT,
	HARBINGER,
	MERCHANT,
	POACHER,
	SENTRY,
	VASSAL,
	SIZE
};

enum class CardType : int {
	TREASURE,
	VICTORY,
	CURSE,
	ACTION,
	ATTACK,
	REACT,
	END
};

class CardData {
public:
	CardData();
	~CardData();

	int key = 0;
	string name = "";
	int cost = 0;
	vector<bool> type;
	vector<string> text;
	int victory = 0;
	bool sVictory = 0;

	vector<int> acts;
	Texture* texture = nullptr;
	POINT covered = { 0,0 }, frame = { 0,0 };
};

class Card : public ImageRect
{
public:
	Card(CardData data);
	~Card();

	void Update() override;
	void Render(HDC hdc) override;

	int GetVictory();

	bool IsType(CardType type) { return data->type[(int)type]; }

	bool isVisible = true;
	bool isCovered = false;
	bool isSelectable = false;
	class MyPointMovement* movement;
	CardData* data;

	function<int(int)> SpecialVictory = nullptr;
};

