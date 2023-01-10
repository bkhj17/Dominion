#include "framework.h"
#include "CardSet.h"
#include "CardSupplier.h"
#include "DominionPlayer.h"
#include "SelectWindow.h"

using namespace ActCondition;


//////////////////////////////////////////////////////////////////////////////

void GetCardResult::Clear()
{
	cards.clear();
	cards.shrink_to_fit();
}

//////////////////////////////////////////////////////////////////////////////

Act::~Act()
{
	DeleteSubAct();
	if (result != nullptr)
		delete result;
}

void Act::Init()
{
	if (result != nullptr)
		result->Clear();
	curSubAct = 0;		//���� ó���� �����Ʈ
	isDoing = false;	//���� �����Ʈ ó����. ���� ��� �� ���
	isDone = false;		//�Ϸ� ����
	isReady = true;		//�غ� �Ϸ�
}

void Act::SetRequested(ActResult* request)
{
	this->requested = request;
}

void Act::Update()
{
	if (!DefaultUpdate()) {
		//���� ���� ��Ʈ�� ���� �� Ȥ�� �Ϸ� �����ΰ�?
		//�ƴ϶�� �غ�
		NextSubAct();
	}
}

//�����Ʈ�� �� �� �ϰ� ��������� �Ǵ�
bool Act::DefaultUpdate()
{
	if (curSubAct == subActs.size()) {
		//��� �۾��� ������
		Done();
		return true;
	}
	if (subActs[curSubAct]->isDone) {
		//���� ���� ��Ʈ�� �Ϸ�� ��Ȳ�̴�
		curSubAct++;
	}
	else if (subActs[curSubAct]->isReady) {
		//�۾� ������
		subActs[curSubAct]->Update();
	}
	else {
		//���� �ߵ� �Ϸ� ���µ� �ƴϴ�
		return false;
	}

	//�������̴�
	return true;
}

void Act::NextSubAct()
{
	//���� �۾��� ����
	if (curSubAct >= subActs.size())
		return;

	//�غ�
	subActs[curSubAct]->Init();
}

void Act::ChangePlayer(DominionPlayer* newPlayer)
{
	//��Ʈ�� �ִ� �÷��̾� �˴� �ٲٱ�
	//�� �ٲܶ� ���
	//ī�� ȿ�� �ߵ��� �Ҷ����� ���� ��Ʈ ���� ����Ƿ� ���� ����
	player = newPlayer;
	for (auto subAct : subActs) {
		subAct->ChangePlayer(newPlayer);
	}
}

int Act::GetPlayerGold()
{
	//�÷��̾� ��
	return player == nullptr ? 0 : player->gold;
}

void Act::Done()
{
	isDoing = false;
	isDone = true;
	requested = nullptr;
}

void Act::DeleteResult()
{
	if (result != nullptr)
		delete result;
}

void Act::DeleteSubAct()
{

	for (auto sub : subActs) {
		delete sub;
	}
	subActs.clear();
}

void Act::SetCurActThis()
{
	DominionGameMaster::Get()->curAct = this;
}

void Act::ReturnCurAct()
{
	DominionGameMaster::Get()->curAct = DominionGameMaster::Get()->mainAct;
}

void Act::Loop()
{
	curSubAct = 0;
	isReady = false;
	isDone = false;
	shutDown = false;
	for (auto sub : subActs) {
		sub->Loop();
	}
}

/// //////////////////////////////////////////////////////////

TurnAct::TurnAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	actType = ActType::TURN;

	subActs.push_back(new TurnStartAct(this, player));
	subActs.push_back(new ActionPhaseAct(this, player));
	subActs.push_back(new BuyPhaseAct(this, player));
	subActs.push_back(new TurnEndAct(this, player));
}

ActionPhaseAct::ActionPhaseAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	actType = ActType::ACTION_PHASE;

	auto a0 = new UseCardFromHandAct(this, player);
	subActs.push_back(a0);
}

void ActionPhaseAct::Init()
{
	Act::Init();

	auto a0 = (UseCardFromHandAct*)subActs[0];
	a0->Init(bind(&IsActionCard, placeholders::_1));
	endCall = false;
}

void ActionPhaseAct::Update()
{
	//���� ���� �� �ϴ���. ���� ������ �˾Ƽ� �ض� ������ ��
	//�׼� �� ��µ� �ڲ� numAction ���δ�....
	//���� �ٲ㼭 ���� ������ �ʴµ� Actionī�� ��� �� �Ѿ...
	if (!subActs[0]->isReady) {
		bool haveAction = player->hand->FindSelectable(&IsActionCard);
		player->hand->SetUnselectable();
		
		if (!haveAction)
			Done();
		else
			subActs[0]->Init();
	}

	auto subAct = (UseCardFromHandAct*)subActs[0];
	subAct->Update();
	if (subAct->isDoing) {
		return;
	} 

	if (subAct->isDone) {
		if (subAct->used == UseCardFromHandAct::Result::USED)
			player->numAction--;
		subAct->Loop();
	}

	//�����ư Ȱ��ȭ
	DominionGameMaster::Get()->SetEndButton(
		"Action Phase End",
		bind(&ActionPhaseAct::EndCall, this));

	if (player->numAction == 0 || endCall) {
		Done();
	}
}

void ActionPhaseAct::Done() { 
	Act::Done();
	DominionGameMaster::Get()->OffEndButton();
	endCall = false; 
}

BuyPhaseAct::BuyPhaseAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	actType = ActType::BUY_PHASE;

	auto a0 = new UseCardFromHandAct(this, player);
	a0->Init([](Card* card) -> bool {
		return card->data->type[(int)CardType::TREASURE];
	});
	subActs.push_back(a0);

	subActs.push_back(new BuyCardAct(this, player));
}

void BuyPhaseAct::Init()
{
	Act::Init();

	endCall = false;
}

void BuyPhaseAct::Update()
{
	auto endButton = DominionGameMaster::Get()->endButton;
	if (player->isAi) {
		if (curSubAct == subActs.size()) {
			endCall = true;
		}
		else {
			subActs[curSubAct]->Update();
			if (subActs[curSubAct]->isDoing)
				return;
			if (subActs[curSubAct]->isDone) {
				if (subActs[curSubAct]->shutDown)
					curSubAct++;
				else
					subActs[curSubAct]->Loop();
			}
		}
	}
	else {
		//���� Act ���� ó��
		endButton->isActive = false;
		for (auto subAct : subActs) {
			subAct->Update();
			if (subAct->isDoing) {
				
				return;
			}
			else if (subAct->isDone) {
				subAct->Loop();
			}
		}
		endButton->isActive = true;
	}

	if (endButton->isActive) {
		endButton->SetText("Buy Phase End");
		endButton->SetEvent(bind(&BuyPhaseAct::EndCall, this));
	}
	if (player->numBuy == 0 || endCall) {
		endButton->isActive = false;
		Done();
	}
}

BuyCardAct::BuyCardAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	subActs.push_back(new GetSupplierAct(this, nullptr));
	subActs.push_back(new SupplyCardAct(this, nullptr));
	subActs.push_back(new InputCardAct(this, nullptr));
}

void BuyCardAct::Init()
{
	shutDown = false;
	Act::Init();
}

void BuyCardAct::NextSubAct()
{
	auto actSelectSupplier = (GetSupplierAct*)subActs[0];
	auto supplyCard = (SupplyCardAct*)subActs[1];
	auto inputToDiscard = (InputCardAct*)subActs[2];
	
	switch (curSubAct) {
	case 0:
	{
		//������ ī�� ����ó ����
		//���� ���� ����
		actSelectSupplier->Init(bind(&BuyCardAct::SelectSupplierCondition, this, placeholders::_1));
	}
		break;
	case 1:
	{
		isDoing = true;
		auto selectedSupplierResult = (GetSupplierResult*)subActs[0]->ReturnResult();
		auto selectedSupplier = selectedSupplierResult->supplier;
		if (selectedSupplier == nullptr) {
			shutDown = true;
			Done();
			return;
		}

		int cost = selectedSupplier->data->cost;
		player->gold -= cost;	//���ź�� ����
		player->numBuy--;		//����Ƚ�� ����

		supplyCard->SetRequested(selectedSupplierResult);
		supplyCard->Init();
	}
		break;
	case 2:
	{
		auto cardResult = (GetCardResult*)supplyCard->ReturnResult();
		inputToDiscard->Init(player->discard, cardResult, false);
		inputToDiscard->isReady = true;
	}
		break;
	}
}

bool BuyCardAct::SelectSupplierCondition(CardSupplier* supplier)
{
	return CostLimit(*(supplier->data), GetPlayerGold());
}

GetSupplierAct::GetSupplierAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	result = new GetSupplierResult();
}

void GetSupplierAct::Init()
{
	this->Init(nullptr);
}

void GetSupplierAct::Init(function<bool(CardSupplier*)> condition)
{
	this->condition = condition;
	result->Clear();
	Act::Init();
}

void GetSupplierAct::Update()
{
	vector<CardSupplier*> selectables = SearchSelectables();
	player->isAi ? AISelect(selectables) : PlayerSelect(selectables);
}

void GetSupplierAct::Done()
{
	for (auto supplier : DominionGameMaster::Get()->suppliers)
		supplier->SetSelectable(nullptr);
	Act::Done();
}

vector<CardSupplier*> GetSupplierAct::SearchSelectables()
{
	vector<CardSupplier*> selectables;
	for (auto supplier : DominionGameMaster::Get()->suppliers) {
		supplier->SetSelectable(condition);
		//ai�� ���ֳ� ��ȭ�� ���� �ʰ� �ϱ� ���� ����(�� �� cost 0)
		bool aiDoNotBuyZero = !(player->isAi && supplier->data->cost == 0);

		if (supplier->IsSelectable() && aiDoNotBuyZero)
			selectables.push_back(supplier);
	}
	return selectables;
}

void GetSupplierAct::AISelect(vector<CardSupplier*>& selectables)
{
	//���� ������ �͵� �� �ƹ��ų� ���´�.
	//0¥���� �� ��´�
	if (!selectables.empty()) {
		int i = rand() % selectables.size();
		((GetSupplierResult*)result)->supplier = selectables[i];
	}

	Done();
}

void GetSupplierAct::PlayerSelect(vector<CardSupplier*>& selectables)
{
	if (!KEY_DOWN(VK_LBUTTON))
		return;

	for (auto supplier : selectables) {
		if (supplier->IsPointCollision(mousePos)) {
			((GetSupplierResult*)result)->supplier = supplier;
			Done();
			break;
		}
	}
}

void GetSupplierResult::Clear()
{
	supplier = nullptr;
}

SupplyCardAct::SupplyCardAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	result = new GetCardResult();
}

void SupplyCardAct::Update()
{
	CardSupplier* supplier = ((GetSupplierResult*)requested)->supplier;
	Card* card = supplier->SupplyCard();

	if (card != nullptr) {
		card->pos = supplier->pos;
		card->isActive = true;
		card->isVisible = true;
		card->isCovered = false;
	}

	result->Clear();
	((GetCardResult*)result)->cards.push_back(card);
	Done();
}

UseCardFromHandAct::UseCardFromHandAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	subActs.push_back(new SelectFromHandAct(this, player));
	subActs.push_back(new InputCardAct(this, player));
	subActs.push_back(new ActiveCardAct(this, player));
}

void UseCardFromHandAct::Init(function<bool(Card*)> conditionFunc)
{
	Act::Init();
	used = Result::NOT;
	condition = conditionFunc;
}

void UseCardFromHandAct::NextSubAct()
{
	if (curSubAct == 0) {
		auto subAct = (SelectFromHandAct*)subActs[0];
		subAct->Init(1, condition);
	}
	else if (curSubAct == 1) {
		isDoing = true;
		//������ ī��� �ν�

		auto preAct = (SelectFromHandAct*)subActs[0];
		//���õ� ī�带 �տ��� ����
		auto& cards = ((GetCardResult*)preAct->ReturnResult())->cards;
		if (cards.empty()) {
			//ī�� ���� �� ��
			shutDown = true;
			Done();
			return;
		}

		for (auto card : cards) {
			player->hand->Out(card);
		}

		auto subAct = (InputCardAct*)subActs[1];
		subAct->SetRequested(preAct->ReturnResult());
		subAct->Init(player->used, (GetCardResult*)preAct->ReturnResult());
	}
	else if (curSubAct == 2) {
		//ī�� ȿ�� �ߵ�
		used = Result::USED;
		auto preAct = (InputCardAct*)subActs[1];
		auto card = ((GetCardResult*)preAct->ReturnResult())->cards[0];

		auto act = (ActiveCardAct*)subActs[2];
		act->Init(card->data->key);
	}
}

InputCardAct::InputCardAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	result = new GetCardResult();
}

void InputCardAct::Init(CardSet* cardSet, GetCardResult* requested, bool toTop, bool visible)
{
	Act::Init();

	this->cardSet = cardSet;
	this->requested = requested;
	this->toTop = toTop;

	curCard = 0;
}

void InputCardAct::Update()
{
	auto& input = ((GetCardResult*)requested)->cards;
	if (curCard == input.size()) {
		//��� ī�尡 �̹� ����ߴ�

		//���� �� �� ī�尡 �ִ��� Ȯ��
		bool yet = false;
		for (auto card : input)
			yet |= card->movement->IsMoving();
		
		if (!yet) {
			//���� �����ߴٸ� Act ����
			auto cardResult = (GetCardResult*)result;
			for (auto card : input)
				cardResult->cards.push_back(card);
			Done();
		}
	}
	else {
		//���� ��� �� �� ī�尡 �ִ�
		time += DELTA;
		if (time >= timeRate) {
			//��� �ð� ��ٷȴ� 1�� ���
			time = 0.0f;
			cardSet->InputCard(input[curCard++], toTop);
		}
	}
}

void InputCardAct::Done()
{
	//���ο� ī�尡 ������ �� �ֱ� ������ ���� ��� 
	Observer::Get()->ExcuteParamEvents("CalcScore", player);
	//������ ���� Ǯ��
	cardSet = nullptr;
	//���� ����
	Act::Done();
}

SelectFromHandAct::SelectFromHandAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	result = new GetCardResult;
}

void SelectFromHandAct::Init(int num, function<bool(Card*)> condition)
{
	selectNum = num;
	this->condition = [this, condition](Card* data) -> bool {
		return selected.size() < selectNum && condition(data); 
	};
	selected.clear();
	result->Clear();

	isReady = true;
}

void SelectFromHandAct::Update()
{
	Card* clicked = nullptr;
	auto hand = player->hand;
	if (!hand->FindSelectable(condition)) {
		Done();
	}

	if (player->isAi) {
		for (auto card : player->hand->cards) {
			if (card->IsSelectable()) {
				clicked = card;
				break;
			}
		}
	}
	else {
		if (KEY_DOWN(VK_LBUTTON)) {
			clicked = hand->GetByPos(mousePos);
			if (clicked != nullptr && !clicked->IsSelectable())
				clicked = nullptr;
		}

		//���ʿ� �� ���ȴ�
		if (clicked == nullptr) {
			return;
		}

		for (int i = 0; i < selected.size(); i++) {
			//������ �� ������
			if (clicked == selected[i]) {
				clicked->SetSelected(false);
				selected.erase(selected.begin() + i);
				clicked = nullptr;
			}
		}
	}

	if (clicked != nullptr && selected.size() < selectNum) {
		clicked->SetSelected(true);
		selected.push_back(clicked);
	}

	if (selectNum == selected.size()) {
		Done();
	}
}

/// <summary>
/// ////////////////////////////////////////////////////////////////////////
/// </summary>

void SelectFromHandAct::Done()
{
	result->Clear();
	auto cardResult = (GetCardResult*)result;
	if (!selected.empty()) {
		cardResult->cards.insert(cardResult->cards.begin(), selected.begin(), selected.end());
		selected.clear();
	}
	player->hand->SetUnselectable();
	Act::Done();
}

void SelectRangeFromHandAct::Init(int minNum, int maxNum, function<bool(Card*)> condition)
{
	if (maxNum == 0) {
		isReady = true;
		isEnd = true;
		Done();
		return;
	}

	SelectFromHandAct::Init(maxNum, condition);
	this->minNum = minNum;
	isEnd = false;

}

void SelectRangeFromHandAct::Update()
{
	if (isDone)
		return;

	SelectFromHandAct::Update();
	if (player->isAi) {
		return;
	}

	if (!player->isAi && !isEnd) {
		isDone = false;
	}

	if (player->IsController()) {
		DominionGameMaster::Get()->SetEndButton(
			END_BUTTON_TEXT,
			bind(&SelectRangeFromHandAct::EndCall, this),
			explain
		);
	}

	if (isEnd) {
		Done();
	}		
}

void SelectRangeFromHandAct::Done()
{
	if (isEnd || player->isAi) {
		DominionGameMaster::Get()->OffEndButton();
		SelectFromHandAct::Done();
	}
}

void SelectRangeFromHandAct::EndCall()
{
	if (selected.size() >= minNum)
		isEnd = true;
}

void SelectRangeFromHandAct::SetEnd(bool end) {
	//�ּ� ���ü����� ���� ���õ��� ��쿡�� ���� ���� ����
	if (selected.size() >= minNum)
		isEnd = end;
}

/// <summary>
////////////////////////////////////////////////////////////////////////
/// </summary>

void GainGoldAct::Init(int n)
{
	num = n;
	isReady = true;
}

void GainGoldAct::Update()
{
	player->gold += num;
	Done();
}

void GainActionAct::Init(int n)
{
	num = n;
	isReady = true;
}

void GainActionAct::Update()
{
	player->numAction += num;
	Done();
}

void GainBuyAct::Init(int n)
{
	num = n;
	isReady = true;
}

void GainBuyAct::Update()
{
	player->numBuy += num;
	Done();
}
/// <summary>
////////////////////////////////////////////////////////////////////////
/// </summary>

void ActiveCardAct::Init(int key)
{
	DeleteSubAct();

	switch (CardKey(key))
	{
	case CardKey::COOPER:
	{
		auto act = new GainGoldAct(parent, player);
		act->Init(1);
		subActs.push_back(act);
	}
	break;
	case CardKey::SILVER:
	{
		auto act = new GainGoldAct(parent, player);
		act->Init(2);
		subActs.push_back(act);
	}
	break;
	case CardKey::GOLD:
	{
		auto act = new GainGoldAct(parent, player);
		act->Init(3);
		subActs.push_back(act);
	}
	break;
	case CardKey::ESTATE:
	case CardKey::DUCHY:
	case CardKey::PROVINCE:
	case CardKey::CURSE:
		break;
		/*
	case CardKey::REMODEL:
		effectAct = new RemodelAct;
		break;
	case CardKey::BUREAUCRAT:
		effectAct = new BureaucratAct;
		break;
	case CardKey::MINE:
		effectAct = new MineAct;
		break;
	case CardKey::MONEYLENDER:
		effectAct = new MoneylenderAct;
		break;
		*/
	case CardKey::SMITHY:	//�Ϸ�
	{
		auto act = new DrawCardAct(this, player);
		act->Init(3);
		subActs.push_back(act);
	}
	break;
	/*
	case CardKey::LIBRARY:
		//�а� 7���� �� ������
		//������ ī�� �̱�.
		//������ �׼�ī���� ��ο� ���� �� ���� ���� ����
		//���� �� �� ī��� ������
		auto act = new LibraryAct;
		break;
	case CardKey::WITCH:
		//���� ����ī��
		effectAct = new WitchAct;
		break;
	*/
	case CardKey::VILLAGE:	//�Ϸ�
	{
		auto act0 = new DrawCardAct(this, player);
		act0->Init(1);
		subActs.push_back(act0);

		auto act1 = new GainActionAct(this, player);
		act1->Init(2);
		subActs.push_back(act1);
	}
	break;
	/*
	case CardKey::MILITIA:
		//�� ��� ����ī��
		effectAct = new MilitiaAct;
		break;
	*/
	case CardKey::MARKET:	//�Ϸ�
	{
		auto act0 = new DrawCardAct(this, player);
		act0->Init(1);
		subActs.push_back(act0);
		auto act1 = new GainActionAct(this, player);
		act1->Init(1);
		subActs.push_back(act1);
		auto act2 = new GainBuyAct(this, player);
		act2->Init(1);
		subActs.push_back(act2);
		auto act3 = new GainGoldAct(this, player);
		act3->Init(1);
		subActs.push_back(act3);
	}
	break;
	case CardKey::LABORATORY:	//�Ϸ�
	{
		auto a0 = new DrawCardAct(this, player);
		a0->Init(2);
		subActs.push_back(a0);
		auto a1 = new GainActionAct(this, player);
		a1->Init(1);
		subActs.push_back(a1);
	}
		break;
	/*
	case CardKey::THRONE_ROOM:
		effectAct = new ThroneRoomAct;
		break;
	case CardKey::CHAPEL:
		effectAct = new ChapelAct;
		break;
	case CardKey::CELLAR:
		effectAct = new CellalAct;
		break;
	*/
	case CardKey::GARDENS:	//�Ϸ�(Ư�� ����)
		break;
		/*
	case CardKey::WORKSHOP:
		effectAct = new WorkShopAct;
		break;
		*/
	case CardKey::FESTIVAL:	//�Ϸ�
	{
		auto a0 = new GainActionAct(this, player);
		a0->Init(2);
		auto a1 = new GainBuyAct(this, player);
		a1->Init(1);
		auto a2 = new GainGoldAct(this, player);
		a2->Init(2);

		subActs.push_back(a0);
		subActs.push_back(a1);
		subActs.push_back(a2);
	}
	break;
	/*
	case CardKey::MOAT:	
	{
		//���� ȿ���� ���� ������ ��
		auto a0 = new DrawCardAct(this, player);
		a0->Init(2);
		subActs.push_back(a0);
	}
	break;
	*/
	case CardKey::COUNCILROOM:	//�Ϸ�
	{
		auto a0 = new DrawCardAct(this, player);
		a0->Init(4);
		subActs.push_back(a0);
		auto a1 = new GainBuyAct(this, player);
		a1->Init(1);
		subActs.push_back(a1);
		for (auto otherPlayer : DominionGameMaster::Get()->players) {
			if (otherPlayer == player)
				continue;
			auto a = new DrawCardAct(this, otherPlayer);
			a->Init(1);
			subActs.push_back(a);
		}
	}
	break;
	case CardKey::ARTISAN:	//�Ϸ�
	{
		subActs.push_back(new ArtisanEffectAct(this, player));
	}
	break;
	/*
	case CardKey::BANDIT:
		effectAct = new BanditAct;
		//�� �ϳ� ������ ī�忡 �ֱ� 
		//����-�� �� �� �� �� �� ����....
		break;
	*/
	case CardKey::HARBINGER:	//�Ϸ�
		//������ ī�忡�� ����ȿ��...
	{
		subActs.push_back(new HarbingerEffectAct(this, player));
	}
		break;
	/*
	case CardKey::MERCHANT:
		effectAct = new MerchantAct;
		//������ ��Ȳ ����ȿ��...
		break;
	*/
	case CardKey::POACHER:	//�Ϸ�
	{
		subActs.push_back(new PoacherEffectAct(this, player));
	}
		break;
	/*
	case CardKey::SENTRY:
		effectAct = new SentryAct;
		break;
	case CardKey::VASSAL:
		effectAct = new VassalAct;
		break;
	*/
	default:
		break;
	}

	isReady = true;
}

DrawCardAct::DrawCardAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	subActs.push_back(new CardFromDeckAct(this, player));
	subActs.push_back(new InputCardAct(this, player));
}

void DrawCardAct::Init(int num)
{
	this->num = num;
	isReady = true;
}

void DrawCardAct::NextSubAct()
{
	if (curSubAct == 0) {
		auto a0 = (CardFromDeckAct*)subActs[0];
		a0->Init(num);
	}
	else if (curSubAct == 1) {
		auto r1 = (GetCardResult*)subActs[0]->ReturnResult();
		auto a2 = (InputCardAct*)subActs[1];
		a2->Init(player->hand, r1);
	}
}

//////////////////////////////////////////////////////////////////////////////

CardFromDeckAct::CardFromDeckAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	result = new GetCardResult;
}

void CardFromDeckAct::Init(int num)
{
	this->num = num;
	result->Clear();

	isReady = true;
}

void CardFromDeckAct::Update() 
{
	//���� ī�忡�� �� ����
	if (player->deck->cards.size() < num)
		player->ReloadDeck();

	auto cardResult = (GetCardResult*)result;
	//�����ߴµ��� ���� ���ٸ� �� ��ο� �� ��
	num = min(num, (int)player->deck->cards.size());

	for (int i = 0; i < num; i++) {
		cardResult->cards.push_back(player->deck->Pop());
	}
	Done(); 
}

//////////////////////////////////////////////////////////////////////////////

TurnEndAct::TurnEndAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	subActs.push_back(new InputCardAct(this, player));
	subActs.push_back(new DrawCardAct(this, player));
	zeroRequest = new GetCardResult;
}

TurnEndAct::~TurnEndAct()
{
	delete zeroRequest;
}

void TurnEndAct::NextSubAct()
{
	if (curSubAct == 0) {
		zeroRequest->Clear();
		auto& hand = player->hand;
		while (!hand->cards.empty()) {
			auto card = hand->Pop();
			zeroRequest->cards.push_back(card);
		}

		auto& used = player->used;
		while (!used->cards.empty()) {
			auto card = used->Pop();
			zeroRequest->cards.push_back(card);
		}

		auto act0 = (InputCardAct*)subActs[0];
		act0->SetTimeRate(0.0f);
		act0->Init(player->discard, zeroRequest);
	}
	else if (curSubAct == 1)
		((DrawCardAct*)subActs[1])->Init(5);
}

bool ActCondition::CostLimit(const CardData& cardData, int limit)
{
	return cardData.cost <= limit;
}

bool ActCondition::IsActionCard(const Card* card)
{
	return card->data->type[(int)CardType::ACTION];
}

void TurnStartAct::Update()
{
	player->TurnStart();
	Done();
}

MainGameAct::MainGameAct()
	: Act(nullptr, nullptr)
{
	actType = ActType::MAIN_GAME;

	subActs.push_back(new TurnAct(this, nullptr));
}

void MainGameAct::Update()
{
	if (curSubAct == 0) {
		if (!subActs[0]->isReady) {
			subActs[0]->ChangePlayer(DominionGameMaster::Get()->GetTurnPlayer());
			subActs[0]->Init();
		}

		subActs[0]->Update();

		if (subActs[0]->isDone) {
			curSubAct++;
		}
	}
	else {
		if (DominionGameMaster::Get()->GameEndTrigger()) {
			Done();
			return;
		}

		DominionGameMaster::Get()->NextTurn();
		subActs[0]->ChangePlayer(DominionGameMaster::Get()->GetTurnPlayer());
		Loop();
	}
}

TrashCardAct::TrashCardAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	result = new GetCardResult();
}

void TrashCardAct::Init(CardSet* cardSet, GetCardResult* request)
{
	auto& trash = DominionGameMaster::Get()->trash;

	result->Clear();
	for (auto card : request->cards) {
		cardSet->Out(card);
		trash->InputCard(card, false, true);
		((GetCardResult*)result)->cards.push_back(card);
	}
	request->Clear();
	Done();
}

SelectFromWindowAct::SelectFromWindowAct(Act* parent, DominionPlayer* player)
	: Act(parent, player) 
{
	result = new GetCardResult();
	unselected = new GetCardResult();
}

void SelectFromWindowAct::Init(GetCardResult* request,
	int minNum, int maxNum,
	function<bool(Card*)> selectableFunc,
	function<void(Card*)> selectFunc,
	function<void()> endFunc)
{
	//result�� unselected�� ���� ����
	result->Clear();
	unselected->Clear();

	if (request == nullptr || request->cards.empty()) {
		Done();
		return;
	}

	SelectWindow::Get()->Init(player, request, (GetCardResult*)result, unselected, selectableFunc, selectFunc, endFunc, !player->IsController());
	SelectWindow::Get()->minNum = minNum;
	SelectWindow::Get()->maxNum = maxNum;

	Act::Init();
}

void SelectFromWindowAct::Update()
{
	if (!SelectWindow::Get()->isActive) { 
		Done();
	}
	else {
		SelectWindow::Get()->Update();
	}
}

PoacherEffectAct::PoacherEffectAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	auto a0 = new DrawCardAct(this, player);
	a0->Init(1);
	auto a1 = new GainActionAct(this, player);
	a1->Init(1);
	auto a2 = new GainGoldAct(this, player);
	a2->Init(1);
	auto a3 = new SelectRangeFromHandAct(this, player);
	auto a4 = new InputCardAct(this, player);

	subActs.push_back(a0);
	subActs.push_back(a1);
	subActs.push_back(a2);
	subActs.push_back(a3);
	subActs.push_back(a4);
}

void PoacherEffectAct::Init()
{
	subActs[3]->Loop();
	Act::Init();
}

void PoacherEffectAct::NextSubAct()
{
	if (curSubAct == 3) {
		int cnt = 0;
		for (const auto& supplier : DominionGameMaster::Get()->suppliers) {
			if (supplier->Empty())
				cnt++;
		}

		auto a3 = (SelectRangeFromHandAct*)subActs[3];
		a3->Init(cnt, cnt, [](Card*) -> bool { return true; });
		string numText = a3->NumRangeText();
		a3->SetExplain(
			"���� ī�带 �����Ͻÿ�(" + a3->NumRangeText() + ")");
	}
	else if (curSubAct == 4) {
		auto a3 = (SelectRangeFromHandAct*)subActs[3];
		auto request = (GetCardResult*)a3->ReturnResult();

		if (request->cards.empty()) {
			Done();
			return;
		}

		for (auto card : request->cards) {
			player->hand->Out(card);
		}
		auto a4 = (InputCardAct*)subActs[4];
		a4->Init(player->discard, request);
	}
	else {
		Act::NextSubAct();
	}
}

HarbingerEffectAct::HarbingerEffectAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	auto a0 = new DrawCardAct(this, player);
	a0->Init(1);
	auto a1 = new GainActionAct(this, player);
	a1->Init(1);

	discards = new GetCardResult();
	auto a2 = new SelectFromWindowAct(this, player);
	auto a3 = new InputCardAct(this, player);

	subActs.push_back(a0);
	subActs.push_back(a1);
	subActs.push_back(a2);
	subActs.push_back(a3);
}

HarbingerEffectAct::~HarbingerEffectAct()
{
	delete discards;
}

void HarbingerEffectAct::NextSubAct()
{
	if (curSubAct == 2) {
		//â�� ������ ī�� ����� ����
		auto a2 = (SelectFromWindowAct*)subActs[2];

		discards->Clear();
		for (auto card : player->discard->cards) {
			discards->cards.push_back(card);
		}

		if (discards->cards.empty()) {
			Done();
			return;
		}

		auto window = SelectWindow::Get();
		a2->Init(discards,
			1, 1,
			bind(&HarbingerEffectAct::Selectable, this, window, placeholders::_1),
			bind(&HarbingerEffectAct::Select, this, placeholders::_1),
			bind(&HarbingerEffectAct::EndSelect, this)			
		);
		window->SetExplain("���� ī�� �� 1�� ��� �� �� ���� ���´�");
	}
	else if(curSubAct == 3) {
		auto a2 = (SelectFromWindowAct*)subActs[2];
		auto r2 = (GetCardResult*)a2->ReturnResult();

		for (auto card : r2->cards) {
			player->discard->Out(card);
		}
		auto a3 = (InputCardAct*)subActs[3];
		a3->Init(player->deck, r2, true, false);
	}
	else {
		Act::NextSubAct();
	}
}

bool HarbingerEffectAct::Selectable(SelectWindow* window, Card* card)
{
	if (window->CurSelectedNum() >= window->maxNum)
		return card->IsSelected();
	else
		return true;
}

void HarbingerEffectAct::Select(Card* card)
{
	card->SetSelected(!card->IsSelected());
}

void HarbingerEffectAct::EndSelect()
{
	auto a2 = (SelectFromWindowAct*)subActs[2];
	auto r2 = (GetCardResult*)a2->ReturnResult();
	for (auto card : discards->cards) {
		if (card->IsSelected())
			r2->cards.push_back(card);
	}
}

ArtisanEffectAct::ArtisanEffectAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	subActs.push_back(new GetSupplierAct(this, player));
	subActs.push_back(new SupplyCardAct(this, player));
	subActs.push_back(new InputCardAct(this, player));
	subActs.push_back(new SelectRangeFromHandAct(this, player));
	subActs.push_back(new InputCardAct(this, player));
}

void ArtisanEffectAct::NextSubAct()
{
	auto a0 = (GetSupplierAct*)subActs[0];
	auto a1 = (SupplyCardAct*)subActs[1];
	auto a2 = (InputCardAct*)subActs[2];
	auto a3 = (SelectRangeFromHandAct*)subActs[3];
	auto a4 = (InputCardAct*)subActs[4];

	if (curSubAct == 0) {
		//�ڽ�Ʈ 5 ������ ī�� 1���� ����ó���� �����´�
		a0->Init([this](CardSupplier* supplier) -> bool {
			return CostLimit(*supplier->data, COST_LIMIT);
		});
		string str = "�ڽ�Ʈ " + to_string(COST_LIMIT) + "������ ī�带 �з� �����´�.";
		DominionGameMaster::Get()->SetExplain(str);
	}
	else if (curSubAct == 1) {
		DominionGameMaster::Get()->SetExplain("");
		a1->SetRequested(a0->ReturnResult());
		a1->Init();
	}
	else if (curSubAct == 2) {

		a2->Init(player->hand, (GetCardResult*)a1->ReturnResult());
		//Result�� �̹� ����Ǿ� ������ �����ϸ� �߸� �ȴ�
	}
	else if (curSubAct == 3) {
		//�п��� ī�� 1���� ��� 
		a3->SetExplain("�� �� ���� ���� ī�� 1���� ���ÿ�");
		a3->Init(1, 1, &ActCondition::IsTrue);
	}
	else if (curSubAct == 4) {
		//�� �� ���� ���´�
		auto r3 = (GetCardResult*)a3->ReturnResult();
		for(auto card : r3->cards)
			player->hand->Out(card);

		a4->Init(player->deck, r3, true);
	}
	else {
		__super::NextSubAct();
	}
}
