#pragma once

class Card;
class GetCardResult;

class SelectWindow : public Rect,  public Singleton<SelectWindow>
{
private:
	friend class Singleton;
	SelectWindow();
	~SelectWindow();
public:
	void Init(GetCardResult* request,
		function<bool(CardData*)> selectableFunc,
		function<void()> selectFunc,
		function<void()> endFunc,
		bool covered = false);

	void Update();
	void Render(HDC hdc);
	void SetExplain(string str) { explain = str; }

	function<bool(CardData*)> selectableFunc = nullptr;
	function<void()> selectFunc = nullptr;	//클릭 시의 행동처리
	function<void()> endFunc = nullptr;			//끝날 때의 결과처리

	const GetCardResult* GetSelected() { return selectedResult; }
	const GetCardResult* GetUnSelected() { return unselectedResult; }
private:
	string explain;
	GetCardResult* request = nullptr;
	GetCardResult* selectedResult;
	GetCardResult* unselectedResult;
	bool covered = false;

	bool isEnd = false;

	int nRect = 0;
	vector<pair<ImageRect*, Card*>> rect;
};

