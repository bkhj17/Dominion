#include "framework.h"
#include "DominionScene.h"
#include "GameMaster.h"
#include "CardManager.h"
#include "CardDataManager.h"
#include "Card.h"
#include "DominionPlayer.h"
#include "InfoBox.h"

DominionScene::DominionScene()
{
	CardDataManager::Get();
	CardManager::Get();
	DominionGameMaster::Get();
	infoBox = new InfoBox();
	infoBox->size = { 300.0f, WIN_HEIGHT-2};
	infoBox->pos = { infoBox->Half().x + 10, CENTER_Y};
}

DominionScene::~DominionScene()
{
	DominionGameMaster::Delete();
	CardManager::Delete();
	CardDataManager::Delete();
}

void DominionScene::Start()
{
	infoBox->Init();
	infoBox->isActive = true;
}

void DominionScene::Update()
{
	DominionGameMaster::Get()->Update();

	infoBox->Update();
	auto mouseOn = DominionGameMaster::Get()->GetMouseOn();
	if (mouseOn != nullptr) {
		infoBox->SetData(mouseOn);
	}

	CardManager::Get()->Update();
}

void DominionScene::Render(HDC hdc)
{
	DominionGameMaster::Get()->Render(hdc);

	infoBox->Render(hdc);

	CardManager::Get()->Render(hdc);

	auto turnPlayer = DominionGameMaster::Get()->GetTurnPlayer();
	wstring wstr = turnPlayer->GetInfo();
	TextOut(hdc, (int)(CENTER_X - 100.0f), (int)(CENTER_Y + 200.0f), wstr.c_str(), (int)wstr.size());
	if (turnPlayer != DominionGameMaster::Get()->GetControlPlayer()) {
		wstr = L"Player" + to_wstring(DominionGameMaster::Get()->turnPlayer) + L"의 턴";
		TextOut(hdc, (int)(CENTER_X - 100.0f), (int)(CENTER_Y + 180.0f), wstr.c_str(), (int)wstr.size());
	}

	//선택창 띄우기
	//curAct->Render
}
