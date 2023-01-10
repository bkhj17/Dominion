#pragma once

class CardData;
class Card;
class CardSet;
class CardSupplier;
class DominionPlayer;

namespace ActCondition {
	//ī�� ������ ���� ����� int ���� ������ true
	bool CostLimit(const CardData&, int);
	inline bool IsTrue(const Card* card) { return true; }
	bool IsActionCard(const Card* card);
};

enum class ActType {
	ACT,
	MAIN_GAME,
	//
	TURN,
	//�� ���� ����
	TURN_START,			//�� ����. �÷��̾� Action 1ȸ, buy 1ȸ, �繰 0
	ACTION_PHASE,
	BUY_PHASE,
	TURN_END,			//

	//ī�� ���� ����
	MOVE_CARD,			//ī�� �̵�

	GET_CARD_LIST,
	SELECT_CARD,		//�÷��̾ ī�� ����

	DRAW_CARD,			//������ �з� ī�� ����
	DISCARD,			//ī�� ������(��𼭰�)
	TRASH_CARD,			//ī�� ���(���������� ��𼭰�)

	SUPPLY_CARD,		//����ó���� ī�� ����

	USE_ACTION_CARD,	//
	ACT_CARD,			//ī�� ȿ�� �ߵ�(�п����� �ߵ��� �� �ִ°� �ƴϴ�)
	USE_TREASURE_CARD,	//
	BUY_CARD,

	//����ī�� ����
	CHECK_REACT,
	COMMAND_REACT_CARD,
};

class Act
{
	friend class MainGameAct;
public:
	Act(Act* parent, DominionPlayer* player) : parent(parent), player(player) {}
	~Act();

	//�ڽ��� �����ϴµ��� �ʿ��� ������ �Է�
	virtual void Init();
	void SetRequested(ActResult* request);	//���� act�� ������ �� �� �ֱ� ������ Init�̶� ������

	virtual void Update();
	bool DefaultUpdate();
	virtual void NextSubAct(); 

	//���� ������� �ʱ�ȭ
	void Loop();

	//�ڽ��� ���� ��� ��ȯ
	ActResult* ReturnResult() { return result; }

	void ChangePlayer(DominionPlayer* newPlayer);
	int GetPlayerGold();
protected:
	virtual void Done();
	void DeleteResult();
	void DeleteSubAct();

	//����! ���� ��Ʈ���� �Ѵ�
	void SetCurActThis();
	void ReturnCurAct();
public:
	//��Ʈ Ÿ��
	ActType actType = ActType::ACT;

	//�غ� ���� : true�� Act ����
	bool isReady = false;
	//��Ʈ ���� ����
	bool shutDown = false;
	//��Ʈ ó�� ��(����ó�� �� �ٸ� ��Ʈ ó�� �� �ϰ� ��)
	bool isDoing = false;
	// Act ���� ����
	bool isDone = false;
protected:
	//���� ó���� 
	int curSubAct = 0;
	//�ڽ��� ���� ��Ʈ
	Act* parent;
	//��Ʈ�� ������ �÷��̾�
	DominionPlayer* player;
	//�ڽ��� ���� ��Ʈ
	deque<Act*> subActs;
	
	ActResult* requested = nullptr;
	ActResult* result = nullptr;
};
 
//���� ��Ʈ : �ֻ��� ��Ʈ. 
class MainGameAct : public Act {
public:
	MainGameAct();

	void Update() override;
};

//�� ��Ʈ : 1�� ����
class TurnAct : public Act {
public:
	TurnAct(Act* parent, DominionPlayer* player);
};

//�� ���� ��Ʈ
class TurnStartAct : public Act {
public:
	TurnStartAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}

	void Update();
};

//�׼� ������ ��Ʈ : �׼� ī�� ���
class ActionPhaseAct : public Act {
public:
	ActionPhaseAct(Act* parent, DominionPlayer* player);

	virtual void Init() override;
	virtual void Update() override;

	void EndCall() { endCall = true; }
	void Done();
private:
	// EndButton�� ���ȴ°�?
	bool endCall = false;
};

class BuyPhaseAct : public Act {
public:
	BuyPhaseAct(Act* parent, DominionPlayer* player);

	virtual void Init() override;
	virtual void Update() override;

	void EndCall() { endCall = true; }
private:
	// EndButton�� ���ȴ°�?
	bool endCall = false;
};

class TurnEndAct : public Act {
public:
	TurnEndAct(Act* parent, DominionPlayer* player);
	~TurnEndAct();

	// Act��(��) ���� ��ӵ�
	virtual void NextSubAct();
private:
	GetCardResult* zeroRequest;
};

class UseCardFromHandAct : public Act {
public:
	enum class Result {
		NOT,
		USED,
		NO_SELECTABLE
	};

	UseCardFromHandAct(Act* parent, DominionPlayer* player);
	void Init(function<bool(Card*)>);
	virtual void NextSubAct();

	function<bool(Card*)> condition;

	Result used = Result::NOT;
};

class BuyCardAct : public Act {
public:
	BuyCardAct(Act* parent, DominionPlayer* player);

	void Init() override;
	void NextSubAct() override;

private:
	bool SelectSupplierCondition(CardSupplier* supplier);


};

//����ó ����
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

//����ó���� ī�� ������
class SupplyCardAct : public Act {
public:
	SupplyCardAct(Act* parent, DominionPlayer* player); 
	// Act��(��) ���� ��ӵ�
	virtual void Update() override;
};

//ī�带 ī��¿� �ֱ�
class InputCardAct : public Act
{
public:
	InputCardAct(Act* parent, DominionPlayer* player);

	void Init(CardSet* cardSet, GetCardResult* requested, bool toTop = false, bool visible = true);
	void SetTimeRate(float timeRate) { this->timeRate = timeRate; }
	void Update();
	void Done() override;

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

	void Init(int num, function<bool(Card*)> condition);
	virtual void Update() override;
	virtual void Done() override;
	int GetSelectNum() { return selectNum; }
protected:
	int selectNum = 0;
	deque<Card*> selected;
	function<bool(Card*)> condition;
};

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

	void EndCall();
protected:
	void SetEnd(bool end);
protected:
	//��ư ����
	const string END_BUTTON_TEXT = "���� �Ϸ�";

	//�ּ� ����
	int minNum = 0;
	//�ִ� ���ڴ� SelectFromHandAct::selectNum�� ���

	//��ư�� ���� ���� ����
	bool isEnd = false;
	//������
	string explain = "";
};

//�繰 ȹ�� ��Ʈ
class GainGoldAct : public Act {
public:
	GainGoldAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}

	void Init(int n);
	void Update();
private:
	int num = 0;
};

//�׼� Ƚ�� ȹ�� ��Ʈ
class GainActionAct : public Act {
public:
	GainActionAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}

	void Init(int n);
	void Update();
private:
	int num = 0;
};

//���� Ƚ�� ȹ�� ��Ʈ
class GainBuyAct : public Act {
public:
	GainBuyAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}

	void Init(int n);
	void Update();
private:
	int num = 0;
};

//ī�� ȿ�� �ߵ�
class ActiveCardAct : public Act {
public:
	ActiveCardAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}

	void Init(int key);
	//void Update();
};

//������ ī�� �������� : ��ο� � ���
class CardFromDeckAct : public Act 
{
public:
	CardFromDeckAct(Act* parent, DominionPlayer* player);

	void Init(int num);
	void Update();

private:
	int num = 0;
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
	//���� �����ϴ� ī��
	//���� ȿ���� �� ���� ��찡 �ִ� - ����
};

class ReaterCardAct : public Act {
	//�⺻ Act�� ���� ���� ȿ���� ���� ���� ī��(����)
public:
	virtual void Init(function<bool(void*)>) = 0;

	vector<Act*> ReactSubActs;

	function<bool(void*)> reactTrigger = nullptr;

	int curReactSub = 0;
};

//Reactable �Լ��� Effect�� ���� ġ��
//curAct ���� ���ͼ�Ʈ �ؼ� �ߵ� ���� Ȯ���ϰ�
//

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


/////////////////////////////////////////////////////////////
//ī�� ȿ�� ���� Act


//������
class SelectWindow;
class HarbingerEffectAct : public Act
{
public:
	HarbingerEffectAct(Act* parent, DominionPlayer* player);
	~HarbingerEffectAct();
	void NextSubAct();
	
private:
	GetCardResult* discards;
	
	bool Selectable(class SelectWindow* window, Card* card);
	void Select(Card* card);
	void EndSelect();
};

//�зƲ�
class PoacherEffectAct : public Act
{
public:
	PoacherEffectAct(Act* parent, DominionPlayer* player);

	void Init() override;
	void NextSubAct() override;
};

//����
class ArtisanEffectAct : public Act 
{
public:
	ArtisanEffectAct(Act* parent, DominionPlayer* player);

	void NextSubAct() override;

private:
	const int COST_LIMIT = 5;
};

