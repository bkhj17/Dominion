#pragma once

class CardData;
class Card;
class CardSet;
class CardSupplier;
class DominionPlayer;

namespace ActCondition {
	//카드 데이터 상의 비용이 int 보다 적으면 true
	bool CostLimit(const CardData&, int);
};

/*
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
}
*/
class ActResult {
public:
	virtual void Clear() = 0;
};

class CardListData {
public:
	deque<Card*> cards;
};

class Act
{
	friend class MainGameAct;
public:
	Act(Act* parent, DominionPlayer* player);
	~Act();

	//자신을 실행하는데에 필요한 데이터 입력
	virtual void Init();
	void SetRequested(ActResult* request);	//앞의 act가 끝나야 쓸 수 있기 때문에 Init이랑 별개로

	virtual void Update();
	bool DefaultUpdate();
	virtual void NextSubAct(); 

	void Loop();

	//자신의 실행 결과 반환
	ActResult* ReturnResult() { return result; }

	void ChangePlayer(DominionPlayer* newPlayer);
	int GetPlayerGold();
protected:
	virtual void Done();
	void DeleteResult();
	void DeleteSubAct();
public:
	bool isReady = false;
	bool isDoing = false;
	bool isDone = false;
protected:
	int curSubAct = 0;
	Act* parent;
	DominionPlayer* player;
	deque<Act*> subActs;

	bool waiting = true;
	
	ActResult* requested = nullptr;
	ActResult* result = nullptr;
};

class MainGameAct : public Act {
public:
	MainGameAct();

	void Update() override;
};


class TurnAct : public Act {
public:
	TurnAct(Act* parent, DominionPlayer* player);
	~TurnAct() = default;
};

class TurnStartAct : public Act {
public:
	TurnStartAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}
	~TurnStartAct() = default;

	void Update();
};

class ActionPhaseAct : public Act {
public:
	ActionPhaseAct(Act* parent, DominionPlayer* player);

	// Act을(를) 통해 상속됨
	void Init() override;
	virtual void Update() override;

	void EndCall() { endCall = true; }
private:
	// EndButton이 눌렸는가?
	bool endCall = false;
};

class BuyPhaseAct : public Act {
public:
	BuyPhaseAct(Act* parent, DominionPlayer* player);
	// Act을(를) 통해 상속됨

	virtual void Init() override;
	virtual void Update() override;

	void EndCall() { endCall = true; }
private:
	// EndButton이 눌렸는가?
	bool endCall = false;
};

//선택된 카드들 목록
class GetCardResult : public ActResult {
public:
	deque<Card*> cards;

	void Clear() override;
};


class TurnEndAct : public Act {
public:
	TurnEndAct(Act* parent, DominionPlayer* player);
	~TurnEndAct();

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

	function<bool(Card*)> condition;
};

class BuyCardAct : public Act {
public:
	BuyCardAct(Act* parent, DominionPlayer* player);
	void Update();
};

//공급처 받기
class GetSupplierAct : public Act {
public:
	GetSupplierAct(Act* parent, DominionPlayer* player);
		//
	virtual void Init() override;
	void Init(function<bool(CardData*)> condition);
	void Update();

private:
	function<bool(CardData*)> condition;
};

//선택된 공급처
class GetSupplierResult : public ActResult {
public:
	CardSupplier* supplier = nullptr;

	void Clear() override;
};

//공급처에서 카드 꺼내기
class SupplyCardAct : public Act {
public:
	SupplyCardAct(Act* parent, DominionPlayer* player); 
	// Act을(를) 통해 상속됨
	virtual void Update() override;
};

class CardMoveAct : public Act {
public:
	CardMoveAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}
	void Init(Vector2 pos, bool isCovered, float timeRate = 0.3f);
	void Update();
private:
	int curCard = 0;
	deque<Card*> cards;
	
	Vector2 pos;

	float time = 0.0f;
	float timeRate = 0.3f;
};

class InputCardAct : public Act
{
public:
	InputCardAct(Act* parent, DominionPlayer* player);
	~InputCardAct();

	//카드 셋으로 클래스 정의?
	void Init(CardSet* cardSet, GetCardResult* requested, bool toTop = false, bool visible = true);
	void SetTimeRate(float timeRate) { this->timeRate = timeRate; }
	void Update();

protected:
	CardSet* cardSet = nullptr;
	int curCard = 0;

	bool toTop = false;

	float time = 0.0f;
	float timeRate = 0.1f;
};

class SelectFromHandAct : public Act 
{
public:
	SelectFromHandAct(Act* parent, DominionPlayer* player);

	void Init(CardSet* hand, int num, function<bool(Card*)> condition);
	void Update();

private:
	CardSet* hand = nullptr;
	int selectNum = 0;
	deque<Card*> selected;
	function<bool(Card*)> condition;
};

class GainGoldAct : public Act {
public:
	GainGoldAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}

	void Init(int n);
	void Update();
private:
	int num = 0;
};


class GainActionAct : public Act {
public:
	GainActionAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}

	void Init(int n);
	void Update();
private:
	int num = 0;
};

class GainBuyAct : public Act {
public:
	GainBuyAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}

	void Init(int n);
	void Update();
private:
	int num = 0;
};

//카드 효과 발동 시키는 놈. 우선 동, 은, 금만 하자
class ActiveCardAct : public Act {
public:
	ActiveCardAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}

	void Init(int key);
	//void Update();
};

class CardFromDeckAct : public Act 
{
public:
	CardFromDeckAct(Act* parent, DominionPlayer* player);
	//;

	void Init(int num);
	void Update() { Done(); }
};

class DrawCardAct : public Act {
public:
	DrawCardAct(Act* parent, DominionPlayer* player);

	void Init(int num);
	void NextSubAct();
private:
	int num = 0;
};

class TrashCardAct : public Act {
public:
	TrashCardAct(Act* parent, DominionPlayer* player);

	void Init(CardSet* cardSet, GetCardResult* request);
};

class AttackAct : public Act {
	//상대방 공격하는 카드
	//공격 효과만 안 들어가는 경우가 있다 - 해자
};

class ReaterCardAct : public Act {
	//기본 Act에 더해 반응 효과가 따로 붙은 카드(해자)
public:
	virtual void Init(function<bool(void*)>) = 0;

	vector<Act*> ReactSubActs;

	function<bool(void*)> reactTrigger = nullptr;

	int curReactSub = 0;
};

//Reactable 함수가 Effect에 들어간다 치자
//curAct 뺏고 인터셉트 해서 발동 여부 확인하고
//




