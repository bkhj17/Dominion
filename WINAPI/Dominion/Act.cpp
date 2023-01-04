#include "framework.h"
#include "Act.h"
#include "Card.h"
#include "CardSet.h"
#include "MyMovement.h"
#include "CardSupplier.h"
#include "GameMaster.h"
#include "DominionPlayer.h"

using namespace ActCondition;

Act::Act(Act* parent, DominionPlayer* player)
	: parent(parent), player(player)
{
}

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
	curSubAct = 0;
	isDoing = false;
	isDone = false;

	isReady = true;
}

void Act::SetRequested(ActResult* request)
{
	this->requested = request;
}

void Act::Update()
{
	if (!DefaultUpdate()) {
		NextSubAct();
	}
}

bool Act::DefaultUpdate()
{
	if (curSubAct == subActs.size()) {
		Done();
		return true;
	}

	if (subActs[curSubAct]->isDone) {
		curSubAct++;
	}
	else if (subActs[curSubAct]->isReady) {
		subActs[curSubAct]->Update();
	}
	else {
		return false;
	}
	return true;
}

void Act::NextSubAct()
{
	if (curSubAct >= subActs.size())
		return;

	subActs[curSubAct]->Init();
}

void Act::ChangePlayer(DominionPlayer* newPlayer)
{
	player = newPlayer;
	for (auto subAct : subActs) {
		subAct->ChangePlayer(newPlayer);
	}
}

int Act::GetPlayerGold()
{
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

void Act::Loop()
{
	curSubAct = 0;
	isReady = false;
	isDone = false;
	for (auto sub : subActs) {
		sub->Loop();
	}
}

/// //////////////////////////////////////////////////////////

TurnAct::TurnAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	subActs.push_back(new TurnStartAct(this, player));
	subActs.push_back(new ActionPhaseAct(this, player));
	subActs.push_back(new BuyPhaseAct(this, player));
	subActs.push_back(new TurnEndAct(this, player));
}

ActionPhaseAct::ActionPhaseAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	auto a0 = new UseCardFromHandAct(this, player);
	subActs.push_back(a0);
}

void ActionPhaseAct::Init()
{
	Act::Init();

	auto a0 = (UseCardFromHandAct*)subActs[0];
	a0->Init([](Card* card) -> bool {
		return card->IsType(CardType::ACTION);
		});
	auto endButton = DominionGameMaster::Get()->endButton;
	endButton->SetText("Action Phase End");
	endButton->SetEvent(bind(&ActionPhaseAct::EndCall, this));
	endCall = false;
}

void ActionPhaseAct::Update()
{
	auto subAct = subActs[0];
	auto endButton = DominionGameMaster::Get()->endButton;

	//여기 병렬 안 하더라. 있으면 알아서 해라 내일의 나
	if (!subAct->isReady)
		subAct->Init();


	subAct->Update();
	if (subAct->isDoing) {
		endButton->isActive = false;
		return;
	}
	else {
		endButton->isActive = true;
	}

	if (subAct->isDone) {
		player->numAction--;
		subAct->Loop();
		return;
	}

	endButton->isActive = true;
	if (player->numAction == 0 || endCall) {
		endButton->isActive = false;
		Done();
	}
}

BuyPhaseAct::BuyPhaseAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	auto a0 = new UseCardFromHandAct(this, player);
	a0->Init([](Card* card) -> bool {
		return card->IsType(CardType::TREASURE);
		});
	subActs.push_back(a0);

	subActs.push_back(new BuyCardAct(this, player));
}

void BuyPhaseAct::Init()
{
	Act::Init();

	auto endButton = DominionGameMaster::Get()->endButton;
	endButton->SetText("Buy Phase End");
	endButton->SetEvent(bind(&BuyPhaseAct::EndCall, this));
	endCall = false;
}

void BuyPhaseAct::Update()
{
	//하위 Act 병렬 처리 
	for (auto subAct : subActs) {
		subAct->Update();
		if (subAct->isDoing)
			return;
		else if (subAct->isDone) {
			subAct->Loop();
			return;
		}
	}

	auto endButton = DominionGameMaster::Get()->endButton;
	endButton->isActive = true;
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

void BuyCardAct::Update()
{
	if (curSubAct == subActs.size()) {
		Done();
		return;
	}
	if (subActs[curSubAct]->isDone) {
		curSubAct++;
		return;
	}

	if (subActs[curSubAct]->isReady) {
		subActs[curSubAct]->Update();
	}
	else {
		if (curSubAct == 0) {
			auto a0 = (GetSupplierAct*)subActs[0];
			//구매 조건을 충족함
			a0->Init([this](CardData* data) -> bool {
				return CostLimit(*data, this->GetPlayerGold());
				});
		}
		else if (curSubAct == 1) {
			isDoing = true;
			auto r0 = (GetSupplierResult*)subActs[0]->ReturnResult();
			int cost = r0->supplier->data->cost;
			player->gold -= cost;	//구매비용 지출
			player->numBuy--;		//구매횟수 저하

			subActs[1]->SetRequested(r0);
			subActs[1]->Init();
			subActs[1]->isReady = true;
		}
		else if (curSubAct == 2) {
			Card* card = ((GetCardResult*)subActs[1]->ReturnResult())->cards[0];

			auto inputAct = (InputCardAct*)subActs[2];
			auto cardResult = (GetCardResult*)subActs[1]->ReturnResult();
			inputAct->Init(player->discard, cardResult, false);
			inputAct->isReady = true;
		}
	}
}

GetSupplierAct::GetSupplierAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	result = new GetSupplierResult();
}

void GetSupplierAct::Init()
{
	this->Init([](CardData*) -> bool { return true; });
}

void GetSupplierAct::Init(function<bool(CardData*)> condition)
{
	this->condition = condition;

	Act::Init();
}

void GetSupplierAct::Update()
{
	if (KEY_DOWN(VK_LBUTTON)) {
		for (auto supplier : DominionGameMaster::Get()->suppliers) {
			if (supplier->IsPointCollision(mousePos)) {
				if (condition != nullptr && !condition(supplier->data))
					continue;

				result->Clear();
				((GetSupplierResult*)result)->supplier = supplier;
				Done();
				break;
			}
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

void CardMoveAct::Init(Vector2 pos, bool isCovered, float timeRate)
{
	Act::Init();

	this->pos = pos;
	cards = ((GetCardResult*)requested)->cards;
	curCard = 0;
	time = this->timeRate = timeRate;
}

void CardMoveAct::Update()
{
	if (curCard == cards.size()) {
		bool yet = false;
		for (auto card : cards) {
			yet |= card->movement->IsMoving();
		}

		if (!yet) {
			DeleteResult();
			auto result = new GetCardResult();
			for (auto card : cards)
				result->cards.push_back(card);
			this->result = result;
			Done();
		}
	}
	else {
		time += DELTA;
		if (time >= timeRate) {
			time = 0.0f;
			cards[curCard]->isVisible = true;
			cards[curCard++]->movement->SetTargetPosByTime(pos, 0.3f);
		}
	}
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
	condition = conditionFunc;
}

void UseCardFromHandAct::NextSubAct()
{
	if (curSubAct == 0) {
		auto subAct = (SelectFromHandAct*)subActs[0];
		subAct->Init(player->hand, 1, condition);
	}
	else if (curSubAct == 1) {
		isDoing = true;
		//선택한 카드들 인식

		auto preAct = (SelectFromHandAct*)subActs[0];
		//선택된 카드를 손에서 제거
		auto cards = ((GetCardResult*)preAct->ReturnResult())->cards;
		if (cards.empty()) {
			Done();
			return;
		}

		for (auto card : cards) {
			player->hand->Out(card);
		}

		auto subAct = (InputCardAct*)subActs[1];
		subAct->SetRequested(preAct->ReturnResult());
		subAct->Init(player->used, (GetCardResult*)preAct->ReturnResult(), true);
	}
	else if (curSubAct == 2) {
		//카드 효과 발동
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

	subActs.push_back(new CardMoveAct(this, player));
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
		bool yet = false;
		for (auto card : input) {
			yet |= card->movement->IsMoving();
		}

		if (!yet) {
			auto cardResult = (GetCardResult*)result;
			for (auto card : input)
				cardResult->cards.push_back(card);
			Done();
		}
	}
	else {
		time += DELTA;
		if (time >= timeRate) {
			time = 0.0f;
			cardSet->InputCard(input[curCard++], toTop);
		}
	}
}

SelectFromHandAct::SelectFromHandAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	result = new GetCardResult;
}

void SelectFromHandAct::Init(CardSet* hand, int num, function<bool(Card*)> condition)
{
	this->hand = hand;
	selectNum = num;
	this->condition = condition;

	isReady = true;
}

void SelectFromHandAct::Update()
{
	Card* clicked = nullptr;
	if (!hand->FindSelectable(condition)) {
		result->Clear();
		Done();
	}

	if (KEY_DOWN(VK_LBUTTON)) {
		clicked = hand->GetByPos(mousePos);
		if (clicked != nullptr && !condition(clicked))
			clicked = nullptr;
	}

	//애초에 안 눌렸다
	if (clicked == nullptr) {
		return;
	}

	for (int i = 0; i < selected.size(); i++) {
		//누른걸 또 눌렀다
		if (clicked == selected[i]) {
			selected.erase(selected.begin() + i);
			return;
		}
	}

	selected.push_back(clicked);

	if (selectNum == selected.size()) {
		result->Clear();
		auto cardResult = (GetCardResult*)result;
		cardResult->cards = selected;
		selected.clear();
		result = cardResult;
		Done();
	}
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
	case CardKey::SMITHY:
	{
		auto act = new DrawCardAct(this, player);
		act->Init(3);
		subActs.push_back(act);
	}
	break;
	/*
case CardKey::LIBRARY:
	auto act = new LibraryAct;
	break;
case CardKey::WITCH:
	effectAct = new WitchAct;
	break;
	*/
	case CardKey::VILLAGE:
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
	effectAct = new MilitiaAct;
	break;
	*/
	case CardKey::MARKET:
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
	/*
case CardKey::LABORATORY:
	effectAct = new LaboratoryAct;
	break;
case CardKey::THRONE_ROOM:
	effectAct = new ThroneRoomAct;
	break;
case CardKey::CHAPEL:
	effectAct = new ChapelAct;
	break;
case CardKey::CELLAR:
	effectAct = new CellalAct;
	break;
case CardKey::GARDENS:
	effectAct = nullptr;
	break;
case CardKey::WORKSHOP:
	effectAct = new WorkShopAct;
	break;
	*/
	case CardKey::FESTIVAL:
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

	case CardKey::MOAT:
	{
		//
	}
	break;

	case CardKey::COUNCILROOM:
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
			auto a = new DrawCardAct(this, player);
			subActs.push_back(a);
		}
	}
	break;
	case CardKey::ARTISAN:
	{
		auto a0 = new GetSupplierAct(this, player);
		a0->Init([](CardData* data) -> bool {
			return CostLimit(*data, 5);
		});
		subActs.push_back(a0);

		auto a1 = new SupplyCardAct(this, player);
		a1->SetRequested(a0->ReturnResult());
		a1->Init();
		subActs.push_back(a1);

		auto a2 = new InputCardAct(this, player);
		a2->Init(player->hand, (GetCardResult*)a1->ReturnResult());
		subActs.push_back(a2);
		//Result가 이미 선언되어 있으니 잘만 된다
	}
	break;
	/*
	case CardKey::BANDIT:
		effectAct = new BanditAct;
		//선택
		break;
	case CardKey::HARBINGER:
		effectAct = new HarbingerAct;
		break;
	case CardKey::MERCHANT:
		effectAct = new MerchantAct;
		break;
	case CardKey::POACHER:
		effectAct = new PoacherAct;
		break;
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
		a2->Init(player->hand, r1, true);
	}
}
CardFromDeckAct::CardFromDeckAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	result = new GetCardResult;
}

void CardFromDeckAct::Init(int num)
{
	result->Clear();

	//버린 카드에서 덱 보충
	if (player->deck->cards.size() < num)
		player->ReloadDeck();

	auto cardResult = (GetCardResult*)result;
	//보충했는데도 덱이 적다면 더 드로우 못 함
	num = min(num, (int)player->deck->cards.size());

	for (int i = 0; i < num; i++) {
		cardResult->cards.push_back(player->deck->Pop());
	}
	isReady = true;
}

void GetCardResult::Clear()
{
	cards.clear();
	cards.shrink_to_fit();
}

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
		act0->Init(player->discard, zeroRequest, false);
	}
	else if (curSubAct == 1)
		((DrawCardAct*)subActs[1])->Init(5);
}

bool ActCondition::CostLimit(const CardData& cardData, int limit)
{
	return cardData.cost <= limit;
}

void TurnStartAct::Update()
{
	player->TurnStart();
	Done();
}

MainGameAct::MainGameAct()
	: Act(nullptr, nullptr)
{
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
		//DominionGameMaster::Get()->NextTurn();
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
