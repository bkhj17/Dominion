#pragma once

class Card;
class CardSupplier;
class DominionPlayer;

enum class ActType {
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

	//�ڽ��� �����ϴµ��� �ʿ��� ������ �Է�
	void Init();
	void SetRequested(ActResult* request);	//���� act�� ������ �� �� �ֱ� ������ Init�̶� ������

	virtual void Update() = 0;
	void Loop();

	//�ڽ��� ���� ��� ��ȯ
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
	// Act��(��) ���� ��ӵ�
	virtual void Update() override;
};

class BuyPhaseAct : public Act {
public:
	BuyPhaseAct(Act* parent, DominionPlayer* player);
	// Act��(��) ���� ��ӵ�
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
	// Act��(��) ���� ��ӵ�
	virtual void Update() override;
};

class TurnAct : public Act {
public:
	virtual void Update() override;;
};

//����ó �ޱ�
class GetSupplierAct : public Act {
public:
	GetSupplierAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}

	void Update();
};

//���õ� ����ó
class GetSupplierResult : public ActResult {
public:
	CardSupplier* supplier;
};

//���õ� ī��� ���
class GetCardResult : public ActResult {
public:
	vector<Card*> cards;
};


//����ó���� ī�� ������
class SupplyCardAct : public Act {
public:
	SupplyCardAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}
	// Act��(��) ���� ��ӵ�
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

	//ī�� ������ Ŭ���� ����?
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

//ī�� ȿ�� �ߵ� ��Ű�� ��. �켱 ��, ��, �ݸ� ����
class ActiveCardAct : public Act {
public:
	ActiveCardAct(Act* parent, DominionPlayer* player);

	void Init(int key);
	void Update();
};