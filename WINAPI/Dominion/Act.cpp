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
}

Act::~Act()
{
	for (auto sub : subActs) {
		delete sub;
	}
}

void Act::Init()
{
	subActNum = 0;
}

void Act::SetRequested(ActResult* request)
{
	this->requested = request;
}

void Act::Done()
{
	isDone = true;
	requested = nullptr;
	GameMaster::Get()->curAct = parent;
}

void Act::CrearData()
{
	delete result;
}

void Act::Loop()
{
	for (auto sub : subActs) {
		sub->Loop();
		sub->isDone = false;

	}
}

void WaitCardMoveAct::Update()
{
	auto cardList = (CardListData*)data;
	bool allMoved = true;
	for (auto card : cardList->cards)
		allMoved &= card->movement->IsMoving();
	
	if (allMoved)
		Done();
}

/// //////////////////////////////////////////////////////////

void TurnAct::Update()
{
}

void ActionPhaseAct::Update()
{
}

void BuyPhaseAct::Update()
{


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
}

void BuyCardAct::Update()
{
	if (subActNum == 0) {
		if (subActs[subActNum]->isDone) {
			subActNum++;
			return;
		}

		GameMaster::Get()->curAct = subActs[0];
	}
	else if (subActNum == 1) {
		if (subActs[subActNum]->isDone) {
			subActNum++;
			return;
		}

		auto req0 = (GetSupplierResult*)subActs[0]->ReturnResult();

		subActs[1]->SetRequested(req0);
		GameMaster::Get()->curAct = subActs[1];

	}
	else if (subActNum == 2) {
		if (subActs[subActNum]->isDone) {
			subActNum++;
			return;
		}

		Card* card = ((GetCardResult*)subActs[1]->ReturnResult())->cards[0];
		card->size = { 100.0f, 150.0f };
		card->movement->SetTargetPosByTime(player->deckRect->pos, 0.3f);
		((CardMoveAct*)subActs[2])->SetRequested(subActs[1]->ReturnResult());
		subActs[2]->Init();
		GameMaster::Get()->curAct = subActs[2];
	}
	else {
		auto cardResult = (GetCardResult*)subActs[2]->ReturnResult();
		player->InsertToDiscard(cardResult->cards[0]);
		Done();
	}
}


void GetSupplierAct::Update()
{
	if (KEY_DOWN(VK_LBUTTON)) {
		for (auto supplier : GameMaster::Get()->suppliers) {
			if (supplier->IsPointCollision(mousePos)) {
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
	result = new GetCardResult();
	((GetCardResult*)result)->cards.push_back(card);
	Done();
}

void CardMoveAct::Init()
{ 
	card = ((GetCardResult*)requested)->cards[0];
}

void CardMoveAct::Update()
{
	if (!card->movement->IsMoving()) {
		auto result = new GetCardResult();
		result->cards.push_back(card);
		this->result = result;
		Done();
	}
}

void UseCardFromHandAct::Update()
{
	for (int i = 0; i < player->hand.size(); i++) {
		if(player->hand[i]->IsPointCollision(mousePos) && condition(player->hand[i])) {
			player->hand[i]->isSelectable = true;
			if (KEY_DOWN(VK_LBUTTON)) {
				//카드 넘겨야지
				auto result = new GetCardResult();
				
				result->cards.push_back(player->hand[i]);
				player->hand.erase(player->hand.begin()+i);

				this->result = result;
				
				Done();
			}
		}
	}
}
