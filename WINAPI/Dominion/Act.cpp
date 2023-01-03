#include "framework.h"
#include "Act.h"
#include "Card.h"
#include "MyMovement.h"
#include "CardSupplier.h"
#include "GameMaster.h"
#include "DominionPlayer.h"

Act::Act(Act* parent, DominionPlayer* player)
	: parent(parent), player(player)
{
	Init();
}

Act::~Act()
{
	for (auto sub : subActs) {
		delete sub;
	}
	DeleteResult();
}

void Act::Init()
{
	curSubAct = 0;
}

void Act::SetRequested(ActResult* request)
{
	this->requested = request;
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

void Act::Loop()
{
	for (auto sub : subActs) {
		sub->Loop();
	}

	curSubAct = 0;
	isReady = false;
	isDone = false;
}

/// //////////////////////////////////////////////////////////

void TurnAct::Update()
{
}

void ActionPhaseAct::Update()
{
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
	if (player->numBuy == 0 /* || EndButton이 눌렸을 때 */) {
		Done();
	}
}

void TurnEndAct::Update()
{
}

BuyCardAct::BuyCardAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	subActs.push_back(new GetSupplierAct(this, nullptr));
	subActs.push_back(new SupplyCardAct(this, nullptr));
	subActs.push_back(new CardMoveAct(this, nullptr));
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
	} else {
		if (curSubAct == 0) {
			subActs[0]->Init();
			subActs[0]->isReady = true;
		}
		else if (curSubAct == 1) {
			isDoing = true;
			auto r0 = (GetSupplierResult*)subActs[0]->ReturnResult();
			int cost = r0->supplier->data->cost;
			if (player->gold < cost) {
				curSubAct = 0;
				subActs[curSubAct]->Loop();
				isDoing = false;
				return;
			}
			else {
				player->gold -= cost;	//구매비용 지출
				player->numBuy--;		//구매횟수 저하

				subActs[1]->SetRequested(r0);
				subActs[1]->Init();
				subActs[1]->isReady = true;
			}
		}
		else if (curSubAct == 2) {
			Card* card = ((GetCardResult*)subActs[1]->ReturnResult())->cards[0];
			card->size = { 100.0f, 150.0f };

			auto act2 = ((CardMoveAct*)subActs[2]);
			act2->SetRequested(subActs[1]->ReturnResult());
			act2->Init(player->deckRect->pos);
			act2->isReady = true;
		}
		else if (curSubAct == 3) {
			auto inputAct = (InputCardAct*)subActs[3];
			auto cardResult = (GetCardResult*)subActs[2]->ReturnResult();
			inputAct->Init(player->discard, cardResult->cards, false);
			inputAct->isReady = true;
		}
	}
}

void GetSupplierAct::Update()
{
	if (KEY_DOWN(VK_LBUTTON)) {
		for (auto supplier : GameMaster::Get()->suppliers) {
			if (supplier->IsPointCollision(mousePos)) {
				DeleteResult();
				result = new GetSupplierResult();
				((GetSupplierResult*)result)->supplier = supplier;
				Done();
				break;
			}
		}
	}
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
	DeleteResult();
	result = new GetCardResult();
	((GetCardResult*)result)->cards.push_back(card);
	Done();
}

void CardMoveAct::Init(Vector2 pos)
{
	this->pos = pos;
	cards = ((GetCardResult*)requested)->cards;
	curCard = 0;
	time = timeRate;
}

void CardMoveAct::Update()
{
	if (curCard == cards.size()) {
		for (auto card : cards) {
			if (!card->movement->IsMoving()) {
				DeleteResult();
				auto result = new GetCardResult();
				result->cards.push_back(card);
				this->result = result;
				Done();
			}
		}
	}
	else {
		time += DELTA;
		if (time >= timeRate) {
			time = 0.0f;
			cards[curCard++]->movement->SetTargetPosByTime(pos, 0.3f);
		}
	}
}

UseCardFromHandAct::UseCardFromHandAct(Act* parent, DominionPlayer* player)
	: Act(parent, player)
{
	subActs.push_back(new SelectFromHandAct(this, player));
	subActs.push_back(new CardMoveAct(this, player));
	subActs.push_back(new InputCardAct(this, player));
	subActs.push_back(new ActiveCardAct(this, player));
}

void UseCardFromHandAct::Init(function<bool(Card*)> conditionFunc)
{
	condition = conditionFunc;

	isReady = true;
}

void UseCardFromHandAct::Update()
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
			auto subAct = (SelectFromHandAct*)subActs[0];
			subAct->Init(player->hand, 1, condition);
		}
		else if (curSubAct == 1) {
			isDoing = true;
			auto preAct = (SelectFromHandAct*)subActs[0];
			auto subAct = (CardMoveAct*)subActs[1];

			//선택된 카드를 손에서 제거
			auto cards = ((GetCardResult*)preAct->ReturnResult())->cards;
			for (auto card : cards) {
				for (auto it = player->hand.begin(); it != player->hand.end(); it++) {
					if (card == *it) {
						player->hand.erase(it);
						break;
					}
				}
			}

			subAct->SetRequested(preAct->ReturnResult());
			subAct->Init(player->usedRect->pos);
			subAct->isReady = true;
		}
		else if (curSubAct == 2) {
			auto preAct = (CardMoveAct*)subActs[1];
			auto subAct = (InputCardAct*)subActs[2];

			subAct->Init(player->used, ((GetCardResult*)preAct->ReturnResult())->cards, true);
		}
		else if (curSubAct == 3) {
			//카드 효과 발동
			auto preAct = (InputCardAct*)subActs[2];
			auto card = ((GetCardResult*)preAct->ReturnResult())->cards[0];

			auto act = (ActiveCardAct*)subActs[3];
			act->Init(card->data->key);
		}
	}
}

void InputCardAct::Init(vector<Card*>& cardSet, vector<Card*> input, bool visible)
{
	for (auto c : input) {
		c->isVisible = visible;
	}

	DeleteResult();
	auto cardResult = new GetCardResult();
	cardResult->cards.insert(cardResult->cards.begin(), input.begin(), input.end());
	result = cardResult;

	cardSet.insert(cardSet.end(), input.begin(), input.end());
	input.clear();

	isReady = true;
}

void InputCardAct::Update()
{
	Done();
}

void SelectFromHandAct::Init(vector<Card*>& cardSet, int num, function<bool(Card*)> condition)
{
	hand = cardSet;
	selectNum = num;
	this->condition = condition;

	isReady = true;
}

void SelectFromHandAct::Update()
{
	Card* clicked = nullptr;

	if (KEY_DOWN(VK_LBUTTON)) {
		for (auto card : hand) {
			if (card->IsPointCollision(mousePos)) {
				if (condition && condition(card)) {
					clicked = card;
					break;
				}
			}
		}
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
		DeleteResult();
		auto cardResult = new GetCardResult;
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

/// <summary>
////////////////////////////////////////////////////////////////////////
/// </summary>

void ActiveCardAct::Init(int key)
{
	for (auto subAct : subActs)
		delete subAct;
	subActs.clear();

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
	case CardKey::SMITHY:
		effectAct = new SmithyAct;
		break;
	case CardKey::LIBRARY:
		effectAct = new LibraryAct;
		break;
	case CardKey::WITCH:
		effectAct = new WitchAct;
		break;
	case CardKey::VILLAGE:
		effectAct = new VillageAct;
		break;
	case CardKey::MILITIA:
		effectAct = new MilitiaAct;
			break;
		case CardKey::MARKET:
			effectAct = new MarketAct;
			break;
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
		case CardKey::FESTIVAL:
			effectAct = new FestivalAct;
			break;
		case CardKey::MOAT:
			effectAct = new MoatAct;
			break;
		case CardKey::COUNCILROOM:
			effectAct = new CouncilRoomAct;
			break;
		case CardKey::ARTISAN:
			effectAct = new ArtisanAct;
			break;
		case CardKey::BANDIT:
			effectAct = new BanditAct;
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

void ActiveCardAct::Update()
{
	if (curSubAct == subActs.size()) {
		Done();
		return;
	}
	if (subActs[curSubAct]->isDone) {
		curSubAct++;
		return;
	}

	if(subActs[curSubAct]->isReady)
		subActs[curSubAct]->Update();
}
