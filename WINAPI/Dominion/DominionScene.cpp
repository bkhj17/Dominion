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
	infoBox->SetPos({ 150.0f, CENTER_Y }, { 300.0f, WIN_HEIGHT - 2 });

	background = new ImageRect(L"Textures/Dominion/Texture/Background.bmp");
	background->size = { WIN_WIDTH, WIN_HEIGHT };
	background->pos = { CENTER_X, CENTER_Y };

	messageBox = new ImageRect(L"Textures/Dominion/Texture/Message.bmp");
	messageBox->pos = { CENTER_X, CENTER_Y };

	font = CreateFont(FONT_HEIGHT, FONT_WIDTH, 0, 0, FW_LIGHT,
		false, false, false, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH, L"배민 을지로10년후체");
}

DominionScene::~DominionScene()
{
	DominionGameMaster::Delete();
	CardManager::Delete();
	CardDataManager::Delete();

	DeleteObject(font);
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
		//게임 종료 시 조치
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
	auto postFont = SelectObject(hdc, font);

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
		wstr = L"Player" + to_wstring(DominionGameMaster::Get()->turnPlayer) + L"의 턴";
		TextOut(hdc, renderX, (int)(CENTER_Y + 180.0f), wstr.c_str(), (int)wstr.size());
	}

	SetTextColor(hdc, BLACK);

	SelectObject(hdc, postFont);
}

void DominionScene::RenderGameEnd(HDC hdc)
{
	messageBox->Render(hdc);

	int postBk = SetBkMode(hdc, 0);
	auto postColor = SetTextColor(hdc, RGB(255, 152, 6));

	wstring strGameEnd = L"게임 종료";
	TextOutW(hdc, (int)(CENTER_X - strGameEnd.size()*(FONT_WIDTH>>1)), (int)CENTER_Y, strGameEnd.c_str(), (int)strGameEnd.size());

	string strWinner = "";

	for (int i = 0; i < winner.size(); i++) {
		strWinner += winner[i]->name + "(" + to_string(winner[i]->GetScore()) + ") ";
	}
	
	strWinner += (winner.size() > 1) ? "비김" : "승리";
	TextOutA(hdc, (int)(CENTER_X - (int)(strWinner.size()-6) * (FONT_WIDTH >> 1)), (int)CENTER_Y + FONT_HEIGHT, strWinner.c_str(), (int)strWinner.size());


	SetBkMode(hdc, postBk);
	SetTextColor(hdc, postColor);
}
