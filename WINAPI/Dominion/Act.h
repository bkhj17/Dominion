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

	virtual void Init();
	//�ڽ��� �����ϴµ��� �ʿ��� ������ �Է�
	void SetRequested(ActResult* request);	//���� act�� ������ �� �� �ֱ� ������ Init�̶� ������

	virtual void Update() {};
	void Done();
	void CrearData();

	void Loop();
	//�ڽ��� ���� ��� ��ȯ
	ActResult* ReturnResult() { return result; }
	
	bool isDone = false;
protected:

	int subActNum = 0;
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
	// Act��(��) ���� ��ӵ�
	virtual void Update() override;
};

class UseCardFromHandAct : public Act {
public:
	UseCardFromHandAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}
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

class WaitCardMoveAct : public Act {
public:
	// Act��(��) ���� ��ӵ�
	virtual void Update() override;
};

//����ó �ޱ�
class GetSupplierAct : public Act {
public:
	GetSupplierAct(Act* parent, DominionPlayer* player) : Act(parent, player) {}

	void Update();
};

class GetSupplierResult : public ActResult {
public:
	CardSupplier* supplier;
};

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
	void Init();
	void Update();

	Card* card = nullptr;
};

class DeckOutAct {

};

class HandOutAct {

};

class DrawCardAct {

};

class GameStartAct {

};