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
	Observer::Get();

	infoBox = new InfoBox();
	infoBox->size = { 300.0f, WIN_HEIGHT-2};
	infoBox->SetPos({ infoBox->Half().x + 10, CENTER_Y });

	background = new ImageRect(L"Textures/Dominion/Texture/Background.bmp");
	background->size = { WIN_WIDTH, WIN_HEIGHT };
	background->pos = { CENTER_X, CENTER_Y };
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

	if (DominionGameMaster::Get()->GetGameState() == DominionGameState::End) {
		//���� ���� �� ��ġ
		winner.clear();
		int max = 0;
		for (auto player : DominionGameMaster::Get()->players) {
			int score = player->GetScore();
			if (score > max) {
				max = score;
				winner.clear();
			}
			
			if (score == max)
				winner.push_back(player);
		}
	}

	CardManager::Get()->Update();
}

void DominionScene::Render(HDC hdc)
{
	background->Render(hdc);
	infoBox->Render(hdc);

	DominionGameMaster::Get()->Render(hdc);



	SetBkMode(hdc, 2);
	SetBkColor(hdc, BLACK);
	SetTextColor(hdc, WHITE);

	int renderX = (int)(CENTER_X - 330.0f);

	auto turnPlayer = DominionGameMaster::Get()->GetTurnPlayer();
	wstring wstr = turnPlayer->GetInfo();
	TextOut(hdc, renderX, (int)(CENTER_Y + 200.0f), wstr.c_str(), (int)wstr.size());
	
	if (DominionGameMaster::Get()->GetGameState() == DominionGameState::End) {
		RenderGameEnd(hdc);
	} else if(turnPlayer != DominionGameMaster::Get()->GetControlPlayer()) {
		wstr = L"Player" + to_wstring(DominionGameMaster::Get()->turnPlayer) + L"�� ��";
		TextOut(hdc, renderX, (int)(CENTER_Y + 180.0f), wstr.c_str(), (int)wstr.size());
	}

	SetTextColor(hdc, BLACK);
}

void DominionScene::RenderGameEnd(HDC hdc)
{
	wstring strGameEnd = L"���� ����";
	TextOutW(hdc, CENTER_X, CENTER_Y, strGameEnd.c_str(), (int)strGameEnd.size());

	string strWinner = "";

	for (int i = 0; i < winner.size(); i++) {
		strWinner += winner[i]->name + "(" + to_string(winner[i]->GetScore()) + ") ";
	}
	
	strWinner += (winner.size() > 1) ? "���" : "�¸�";
	
	TextOutA(hdc, CENTER_X, CENTER_Y + 20, strWinner.c_str(), (int)strWinner.size());
}
