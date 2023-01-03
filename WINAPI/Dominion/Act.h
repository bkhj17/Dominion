#pragma once

class Card;
class CardSupplier;
class DominionPlayer;

enum class ActType {
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

struct ActData {
	ActType type;
	vector<ActType> subType;
};

class ActResult {
};

class CardListData {
public:
	vector<Card*> cards;
};

class Act
{
public:
	Act(Act* parent, DominionPlayer* player);
	~Act();

	//자신을 실행하는데에 필요한 데이터 입력
	void Init();
	void SetRequested(ActResult* request);	//앞의 act가 끝나야 쓸 수 있기 때문에 Init이랑 별개로

	virtual void Update() = 0;
	void Loop();

	//자신의 실행 결과 반환
	ActResult* ReturnResult() { return result; }
protected:
	void Done();
	void DeleteResult();

public:
	bool isReady = false;
	bool isDone = false;
protected:

	int curSubAct = 0;
	Act* parent;
	DominionPlayer* player;
	vector<Act*> subActs;

	bool waiting = true;
	
	ActData* data = nullptr;
	ActResult* requested = nullptr;
	ActResult* result = nullptr;
};

class ActionPhaseAct : public Act {
	// Act을(를) 통해 상속됨
	virtual void Update() override;
};

class BuyPhaseAct : public Act {
public:
	BuyPhaseAct(Act* parent, DominionPlayer* player);
	// Act을(를) 통해 상속됨
	virtual void Update() override;
};

class UseCardFromHandAct : public Act {
public:
	UseCardFromHandAct(Act* parent, DominionPlayer* player);
	void Init(function<bool(Card*)>);
	virtual void Update() override;

	function<bool(Card*)> condition;
};

class BuyCardAct : public Act {
public:
	BuyCardAct(Act* parent, DominionPlayer* player);
	void Update();
};

class TurnEndAct : public Act {
	// Act을(를) 통해 상속됨
	virtual void Update() override;
};

class TurnAct : public Act {
public:
	virtual void Update() override;;
};

//공급처 받기
class GetSupplierAct : public Act {
public:
	GetSupplierAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}

	void Update();
};

//선택된 공급처
class GetSupplierResult : public ActResult {
public:
	CardSupplier* supplier;
};

//선택된 카드들 목록
class GetCardResult : public ActResult {
public:
	vector<Card*> cards;
};


//공급처에서 카드 꺼내기
class SupplyCardAct : public Act {
public:
	SupplyCardAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}
	// Act을(를) 통해 상속됨
	virtual void Update() override;
};

class CardMoveAct : public Act {
public:
	CardMoveAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}
	void Init(Vector2 pos);
	void Update();

private:
	int curCard = 0;
	vector<Card*> cards;
	
	Vector2 pos;

	float time = 0.0f;
	float timeRate = 0.3f;
};

class CardToHandAct : public Act 
{
public:
	CardToHandAct(Act* parent, DominionPlayer* player);
	void SetRequested(ActResult* request);
	void Update();
};

class InputCardAct : public Act
{
public:
	InputCardAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}

	//카드 셋으로 클래스 정의?
	void Init(vector<Card*>& cardSet, vector<Card*> input, bool visible = true);
	void Update();
};

class SelectFromHandAct : public Act 
{
public:
	SelectFromHandAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}

	void Init(vector<Card*>& cardSet, int num, function<bool(Card*)> condition);
	void Update();

private:
	vector<Card*> hand;
	int selectNum = 0;
	vector<Card*> selected;
	function<bool(Card*)> condition;
};

class GainGoldAct : public Act {
public:
	GainGoldAct(Act* parent, DominionPlayer* player);

	void Init(int n);
	void Update();
private:
	int num = 0;
};

//카드 효과 발동 시키는 놈. 우선 동, 은, 금만 하자
class ActiveCardAct : public Act {
public:
	ActiveCardAct(Act* parent, DominionPlayer* player);

	void Init(int key);
	void Update();
};