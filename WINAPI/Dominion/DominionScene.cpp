#include "framework.h"
#include "DominionScene.h"
#include "GameMaster.h"
#include "CardManager.h"
#include "CardDataManager.h"
#include "Card.h"
#include "DominionPlayer.h"

DominionScene::DominionScene()
{
	CardDataManager::Get();
	CardManager::Get();
	DominionGameMaster::Get();
	infoBox = new Rect(100.0f, 260.0f, 220.0f, 440.0f);
}

DominionScene::~DominionScene()
{
	DominionGameMaster::Delete();
	CardManager::Delete();
	CardDataManager::Delete();
}

void DominionScene::Start()
{
}

void DominionScene::Update()
{
	DominionGameMaster::Get()->Update();

	auto mouseOn = DominionGameMaster::Get()->GetMouseOn();
	if (mouseOn != nullptr) {
		dataInfo = mouseOn;
	}

	CardManager::Get()->Update();
}

void DominionScene::Render(HDC hdc)
{
	DominionGameMaster::Get()->Render(hdc);

	if (dataInfo != nullptr)
		dataInfo->texture->Render(hdc, infoBox, dataInfo->frame);
	else
		CardDataManager::Get()->RenderCovered(hdc, infoBox);

	CardManager::Get()->Render(hdc);

	auto turnPlayer = DominionGameMaster::Get()->GetTurnPlayer();
	wstring wstr = turnPlayer->GetInfo();
	TextOut(hdc, (int)(CENTER_X - 100.0f), (int)(CENTER_Y + 200.0f), wstr.c_str(), (int)wstr.size());
	if (turnPlayer != DominionGameMaster::Get()->GetControlPlayer()) {
		wstr = L"Player" + to_wstring(DominionGameMaster::Get()->turnPlayer) + L" 진행 중. 기다리시오.";
		TextOut(hdc, (int)(CENTER_X - 100.0f), (int)(CENTER_Y + 200.0f), wstr.c_str(), (int)wstr.size());
	}
}
