#include "framework.h"
#include "Act.h"
#include "Card.h"
#include "MyMovement.h"
#include "CardSupplier.h"
#include "GameMaster.h"

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
}

void Act::SetRequested(ActResult* request)
{
	this->requested = request;
}

void Act::Done()
{
	isDone = true;
	GameMaster::Get()->curAct = parent;
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

TestAct::TestAct()
	: Act(nullptr, nullptr)
{
	subActs.push_back(new GetSupplierAct(this, nullptr));
	subActs.push_back(new SupplyCardAct(this, nullptr));
	subActs.push_back(new CardMoveAct(this, nullptr));
}

void TestAct::Update()
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

		((CardMoveAct*)subActs[2])->card = ((GetCardResult*)subActs[1]->ReturnResult())->card;
		((CardMoveAct*)subActs[2])->card->size = { 100.0f, 150.0f };
		((CardMoveAct*)subActs[2])->pos = { 100.0f, WIN_HEIGHT - 60.0f };
		subActs[2]->Init();
		GameMaster::Get()->curAct = subActs[2];
	}
	else {
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
	((GetCardResult*)result)->card = card;
	Done();
}


void CardMoveAct::Init()
{
	card->movement->SetTargetPosByTime(pos);
}

void CardMoveAct::Update()
{
	if (!card->movement->IsMoving())
		Done();
}