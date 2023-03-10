#pragma once

class CardData;
class Card;
class CardSet;
class CardSupplier;
class DominionPlayer;

//condition용으로 자주 사용 되는 함수들
namespace ActCondition {
	//카드 데이터 상의 비용이 int 보다 적으면 true
	bool CostLimit(const CardData&, int);
	bool CostLimit(const Card*, int);
	inline bool IsTrue(const Card* card) { return true; }
	bool IsActionCard(const Card* card);
};

enum class ActType {
	ACT,
	MAIN_GAME,
	//
	TURN,
	//턴 진행 관련
	TURN_START,			//턴 시작. 플레이어 Action 1회, buy 1회, 재물 0
	ACTION_PHASE,
	BUY_PHASE,
	TURN_END,			//

	//카드 제어 관련
	MOVE_CARD,			//카드 이동

	GET_CARD_LIST,
	SELECT_CARD,		//플레이어가 카드 선택

	DRAW_CARD,			//덱에서 패로 카드 집기
	DISCARD,			//카드 버리기(어디서건)
	TRASH_CARD,			//카드 폐기(마찬가지로 어디서건)

	SUPPLY_CARD,		//공급처에서 카드 생성

	USE_ACTION_CARD,	//
	ACT_CARD,			//카드 효과 발동(패에서만 발동할 수 있는게 아니다)
	USE_TREASURE_CARD,	//
	BUY_CARD,

	//반응카드 관련
	CHECK_REACT,
	COMMAND_REACT_CARD,
};

class Act
{
	friend class MainGameAct;
public:
	Act(Act* parent, DominionPlayer* player) : parent(parent), player(player) {}
	virtual ~Act();

	//자신을 실행하는데에 필요한 데이터 입력
	virtual void Init();
	void SetRequested(ActResult* request);	//앞의 act가 끝나야 쓸 수 있기 때문에 Init이랑 별개로

	virtual void Update();
	bool DefaultUpdate();
	virtual void NextSubAct(); 

	//액터 진행상태 초기화
	void Loop();

	//자신의 실행 결과 반환
	ActResult* ReturnResult() { return result; }

	void ChangePlayer(DominionPlayer* newPlayer);
	int GetPlayerGold();
protected:
	virtual void Done();
	void DeleteResult();
	void DeleteSubAct();

	//주의! 둘은 세트여야 한다
	void SetCurActThis();
	void ReturnCurAct();
public:
	//액트 타입
	ActType actType = ActType::ACT;

	//준비 여부 : true면 Act 실행
	bool isReady = false;
	//액트 루프 없음
	bool shutDown = false;
	//액트 처리 중(병렬처리 중 다른 액트 처리 안 하게 함)
	bool isDoing = false;
	// Act 종료 여부
	bool isDone = false;
protected:
	//현재 처리할 
	int curSubAct = 0;
	//자신의 상위 액트
	Act* parent;
	//액트를 실행할 플레이어
	DominionPlayer* player;
	//자신의 하위 액트
	deque<Act*> subActs;
	
	ActResult* requested = nullptr;
	ActResult* result = nullptr;
};
 
//게임 액트 : 최상위 액트. 
class MainGameAct : public Act {
public:
	MainGameAct();

	void Update() override;
};

//턴 액트 : 1턴 진행
class TurnAct : public Act {
public:
	TurnAct(Act* parent, DominionPlayer* player);
};

//턴 시작 액트
class TurnStartAct : public Act {
public:
	TurnStartAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}

	void Update();
};

//액션 페이즈 액트 : 액션 카드 사용
class ActionPhaseAct : public Act {
public:
	ActionPhaseAct(Act* parent, DominionPlayer* player);

	virtual void Init() override;
	virtual void Update() override;

	void EndCall() { endCall = true; }
	void Done();
private:
	// EndButton이 눌렸는가?
	bool endCall = false;
};

class BuyPhaseAct : public Act {
public:
	BuyPhaseAct(Act* parent, DominionPlayer* player);

	virtual void Init() override;
	virtual void Update() override;

	void EndCall() { endCall = true; }
private:
	// EndButton이 눌렸는가?
	bool endCall = false;
};

class TurnEndAct : public Act {
public:
	TurnEndAct(Act* parent, DominionPlayer* player);
	virtual ~TurnEndAct();

	// Act을(를) 통해 상속됨
	virtual void NextSubAct();
private:
	GetCardResult* zeroRequest;
};

class UseCardFromHandAct : public Act {
public:
	UseCardFromHandAct(Act* parent, DominionPlayer* player);
	void Init(function<bool(Card*)>);
	virtual void NextSubAct();

protected:
	function<bool(Card*)> condition;
};

class BuyCardAct : public Act {
public:
	BuyCardAct(Act* parent, DominionPlayer* player);

	void NextSubAct() override;
private:
	//공급처 선택 조건 함수. 플레이어 재물보다 적어야 함
	bool SelectSupplierCondition(CardSupplier* supplier);
};

//공급처 선택
class GetSupplierAct : public Act {
public:
	GetSupplierAct(Act* parent, DominionPlayer* player);
	//
	virtual void Init() override;
	void Init(function<bool(CardSupplier*)> condition);
	void Update();

	void Done() override;
private:
	vector<CardSupplier*> SearchSelectables();
	void AISelect(vector<CardSupplier*>& selectables);
	void PlayerSelect(vector<CardSupplier*>& selectables);

private:
	function<bool(CardSupplier*)> condition;
};

//공급처에서 카드 꺼내기
class SupplyCardAct : public Act {
public:
	SupplyCardAct(Act* parent, DominionPlayer* player); 

	virtual void Update() override;
};

//카드를 카드셋에 넣기
class InputCardAct : public Act
{
public:
	InputCardAct(Act* parent, DominionPlayer* player);

	virtual void Init(CardSet* cardSet, GetCardResult* requested, bool toTop = false, bool visible = true);
	virtual void Update() override;
	virtual void Done() override;
	//카드 이동 빈도 설정
	void SetTimeRate(float timeRate) { this->timeRate = timeRate; }
protected:
	CardSet* cardSet = nullptr;
	int curCard = 0;

	bool toTop = false;

	float time = 0.0f;
	float timeRate = 0.1f;
};

//패에서 카드 선택
class SelectFromHandAct : public Act 
{
public:
	SelectFromHandAct(Act* parent, DominionPlayer* player);

	void Init(int num, function<bool(Card*)> condition);
	virtual void Update() override;
	virtual void Done() override;
	int GetSelectNum() { return selectNum; }
protected:
	//선택할 수량
	int selectNum = 0;
	//선택된 카드 목록
	deque<Card*> selected;
	//선택 가능 여부 함수
	function<bool(Card*)> condition;
};

//패에서 일정 범위 수 카드 선택 - 버튼에 의한 임의 종료 등 추가
class SelectRangeFromHandAct : public SelectFromHandAct
{
public:
	SelectRangeFromHandAct(Act* parent, DominionPlayer* player) : SelectFromHandAct(parent, player) {}
	virtual void Init(int minNum, int maxNum, function<bool(Card*)> condition);
	virtual void Update() override;
	virtual void Done() override;

	void SetExplain(string str) { explain = str; }
	int GetMinNum() { return minNum; }
	
	string NumRangeText() { 
		return minNum == selectNum ? to_string(selectNum)
			: to_string(minNum) + "~" + to_string(selectNum);
	}

	//종료 선언
	void EndCall();
protected:
	void SetEnd(bool end);
protected:
	//버튼 문구
	const string END_BUTTON_TEXT = "선택 완료";

	//최소 숫자
	int minNum = 0;
	//최대 숫자는 SelectFromHandAct::selectNum을 사용

	//버튼에 의한 종료 여부
	bool isEnd = false;
	//설명문구
	string explain = "";
};

//재물 획득 액트
class GainGoldAct : public Act {
public:
	GainGoldAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}

	void Init(int n);
	void Update();
private:
	int num = 0;
};

//액션 횟수 획득 액트
class GainActionAct : public Act {
public:
	GainActionAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}

	void Init(int n);
	void Update();
private:
	int num = 0;
};

//구매 횟수 획득 액트
class GainBuyAct : public Act {
public:
	GainBuyAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}

	void Init(int n);
	void Update();
private:
	int num = 0;
};

//카드 효과 발동
class ActiveCardAct : public Act {
public:
	ActiveCardAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}

	void Init(int key);
	//void Update();
};

//덱 맨 위에서 카드 목록 불러오기 : 드로우 등에 사용
class CardFromDeckAct : public Act 
{
public:
	CardFromDeckAct(Act* parent, DominionPlayer* player);

	void Init(int num);
	void Update();
private:
	int num = 0;
};

//덱에서 카드 드로우
class DrawCardAct : public Act {
public:
	DrawCardAct(Act* parent, DominionPlayer* player);

	void Init(int num);
	void NextSubAct();
private:
	int num = 0;
};

//창에서 선택 Act
class SelectFromWindowAct : public Act {
public:	
	SelectFromWindowAct(Act* parent, DominionPlayer* player);

	void Init(GetCardResult* request,
		int minNum, int maxNum,
		function<bool(Card*)> selectableFunc,
		function<void(Card*)> selectFunc,
		function<void()> endFunc);
	void Update();

public:
	GetCardResult* unselected;
};


/* 
//미구현
class AttackAct : public Act {
	//상대방 공격하는 카드
	//공격 효과만 안 들어가는 경우가 있다 - 해자
};

class ReaterCardAct : public Act {
	//기본 Act에 더해 반응 효과가 따로 붙은 카드(해자 등)
public:
	virtual void Init(function<bool(void*)>) = 0;
	vector<Act*> ReactSubActs;
	function<bool(void*)> reactTrigger = nullptr;
	int curReactSub = 0;
};
*/

/////////////////////////////////////////////////////////////
//카드 효과 전용 Act


//선구자
class SelectWindow;
class HarbingerEffectAct : public Act
{
public:
	HarbingerEffectAct(Act* parent, DominionPlayer* player);
	virtual ~HarbingerEffectAct();
	void NextSubAct();
	
private:
	GetCardResult* discards;
	
	bool Selectable(class SelectWindow* window, Card* card);
	void Select(Card* card);
	void EndSelect();
};

//밀렵꾼
class PoacherEffectAct : public Act
{
public:
	PoacherEffectAct(Act* parent, DominionPlayer* player);

	void Init() override;
	void NextSubAct() override;
};

//장인
class ArtisanEffectAct : public Act 
{
public:
	ArtisanEffectAct(Act* parent, DominionPlayer* player);

	void NextSubAct() override;

private:
	const int COST_LIMIT = 5;
};

