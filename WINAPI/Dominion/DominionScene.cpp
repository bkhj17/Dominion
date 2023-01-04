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
	GameMaster::Get();
	infoBox = new Rect(100.0f, 260.0f, 220.0f, 440.0f);
}

DominionScene::~DominionScene()
{
	GameMaster::Delete();
	CardManager::Delete();
	CardDataManager::Delete();
}

void DominionScene::Start()
{
}

void DominionScene::Update()
{
	GameMaster::Get()->Update();

	auto mouseOn = GameMaster::Get()->GetMouseOn();
	if (mouseOn != nullptr) {
		dataInfo = mouseOn;
	}

	CardManager::Get()->Update();
}

void DominionScene::Render(HDC hdc)
{
	GameMaster::Get()->Render(hdc);

	if (dataInfo != nullptr)
		dataInfo->texture->Render(hdc, infoBox, dataInfo->frame);
	else
		CardDataManager::Get()->RenderCovered(hdc, infoBox);

	CardManager::Get()->Render(hdc);

	auto turnPlayer = GameMaster::Get()->GetTurnPlayer();
	wstring wstr = turnPlayer->GetInfo();
	TextOut(hdc, (int)(CENTER_X - 100.0f), (int)(CENTER_Y + 200.0f), wstr.c_str(), (int)wstr.size());
	if (turnPlayer != GameMaster::Get()->GetControlPlayer()) {
		wstr = L"Player" + to_wstring(GameMaster::Get()->turnPlayer) + L" 진행 중. 기다리시오.";
		TextOut(hdc, (int)(CENTER_X - 100.0f), (int)(CENTER_Y + 200.0f), wstr.c_str(), (int)wstr.size());
	}
}
