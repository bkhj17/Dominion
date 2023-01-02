#include "framework.h"
#include "DominionScene.h"
#include "GameMaster.h"
#include "CardManager.h"
#include "Card.h"

DominionScene::DominionScene()
{
	CardManager::Get();
	GameMaster::Get();
	infoBox = new Rect(100.0f, 260.0f, 220.0f, 440.0f);

}

DominionScene::~DominionScene()
{
	GameMaster::Delete();
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

	if(dataInfo != nullptr)
		dataInfo->texture->Render(hdc, infoBox, dataInfo->frame);

	CardManager::Get()->Render(hdc);
}
