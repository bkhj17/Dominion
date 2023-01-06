#pragma once

enum class CardKey : int {
	COOPER = 0,		//동화
	SILVER,			//은화
	GOLD,			//금화
	ESTATE,			//사유지
	DUCHY,			//공작령
	PROVINCE,		//속주
	CURSE,			//저주
	REMODEL,		//개조
	BUREAUCRAT,		//관료
	MINE,			//광산
	MONEYLENDER,	//대금업자
	SMITHY,			//대장장이
	LIBRARY,		//도서관
	WITCH,			//마녀
	VILLAGE,		//마을
	MILITIA,		//민병대
	MARKET,			//시장
	LABORATORY,		//실험실
	THRONE_ROOM,	//알현실
	CHAPEL,			//예배당
	CELLAR,			//저장고
	GARDENS,		//정원
	WORKSHOP,		//작업장
	FESTIVAL,		//축제
	MOAT,			//해자
	COUNCILROOM,	//회의실
	ARTISAN,		//장인
	BANDIT,			//노상강도
	HARBINGER,		//선구자
	MERCHANT,		//상인
	POACHER,		//밀렵꾼
	SENTRY,			//보초병
	VASSAL,			//신하
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
	CardData() = default;
	~CardData() = default;

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
	friend class CardManager;
public:
	static const Vector2 DEFAULT_SIZE;
	
public:
	Card(CardData* data);
	~Card();

	void Update() override;
	void Render(HDC hdc) override;
	void Render(HDC hdc, Rect* renderPos, bool covered);

	bool IsType(CardType type) { return data->type[(int)type]; }

	bool isVisible = true;
	bool isCovered = false;
	class MyPointMovement* movement;
	CardData* data;

	function<int(int)> SpecialVictory = nullptr;

	void SetSelectable(function<bool(Card*)> condition);
	bool IsSelectable() { return isSelectable; }

	void SetSelected(bool select) { isSelected = select; }
	bool IsSelected() { return isSelected; }
private:
	bool isSelectable = false;
	HPEN selectablePen = nullptr;

	bool isSelected = false;
	HPEN selectedPen = nullptr;
};

