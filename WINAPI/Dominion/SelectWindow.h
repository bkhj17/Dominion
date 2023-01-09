#pragma once

class Card;
class GetCardResult;

class SelectWindow : public ImageRect,  public Singleton<SelectWindow>
{
private:
	friend class Singleton;
	SelectWindow();
	~SelectWindow();
public:
	void Init(DominionPlayer* player, 
		GetCardResult* request,
		GetCardResult* selected,
		GetCardResult* unselected,
		function<bool(Card*)> selectableFunc,
		function<void(Card*)> selectFunc,
		function<void()> endFunc,
		bool covered = false);
	void SetResize(Vector2 pos, Vector2 size);

	void Done();
	void EndCall();

	void Update();
	void Render(HDC hdc);
	void SetExplain(string str) { explain = str; }

	int CurSelectedNum();

	const GetCardResult* GetSelected() { return selectedResult; }
	const GetCardResult* GetUnSelected() { return unselectedResult; }

	Card* GetCardMouseOn();

	void SortRects();
	void MoveRects();
	bool IsClickable(int i);

	int minNum = 0, maxNum = 0;

private:
	DominionPlayer* player = nullptr;

	string explain;
	GetCardResult* request = nullptr;
	GetCardResult* selectedResult = nullptr;
	GetCardResult* unselectedResult = nullptr;
	bool covered = false;

	bool isEnd = false;

	int nRect = 0;
	vector<pair<ImageRect*, Card*>> cardRect;

	function<bool(Card*)> selectableFunc = nullptr;	//
	function<void(Card*)> selectFunc = nullptr;			//클릭 시의 행동처리
	function<void()> endFunc = nullptr;				//끝날 때의 결과처리

	ImageRect* leftTab;
	ImageRect* rightTab;

	float waitTime = 0.0f;
	float waitRate = 0.5f;
};

