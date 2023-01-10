#include "framework.h"
#include "Card.h"
#include "CardDataManager.h"
#include "InfoBox.h"
#include "GameMaster.h"
#include "DominionPlayer.h"
#include "SelectWindow.h"

InfoBox::InfoBox()
	: ImageRect(L"Textures/Dominion/Texture/InfoBox.bmp")
{
	showImage = new Rect(IMAGE_POS, IMAGE_SIZE);
	scoreBox = new ImageRect(L"Textures/Dominion/Texture/ScoreBox.bmp");
}

InfoBox::~InfoBox()
{
	delete showImage;
	delete scoreBox;
}

void InfoBox::Init()
{
	dataInfo = nullptr;
}

void InfoBox::Update()
{
	//마우스에 새로 잡힌게 있는지 확인
	const CardData* mouseOn = nullptr;
	if (SelectWindow::Get()->isActive) {
		//선택 창 열림
		auto card = SelectWindow::Get()->GetCardMouseOn();
		if(card != nullptr)
			mouseOn = card->data;
	} 
	else //아니면 통상으로
		mouseOn = DominionGameMaster::Get()->GetMouseOn();

	//있으면 띄울 정보 변경 
	if (mouseOn != nullptr)
		SetData(mouseOn);
}

void InfoBox::Render(HDC hdc)
{
	int postMode = SetBkMode(hdc, 0);
	__super::Render(hdc);

	if (dataInfo != nullptr) {
		dataInfo->texture->Render(hdc, showImage, dataInfo->frame);
		RenderText(hdc);
	}
	else
		CardDataManager::Get()->RenderCovered(hdc, showImage);

	RenderScore(hdc);
	SetBkMode(hdc, postMode);
}

void InfoBox::SetPos(Vector2 pos)
{
	this->pos = pos;
	showImage->pos = { pos.x + IMAGE_POS.x, Top() + showImage->Half().y + IMAGE_POS.y };
	scoreBox->pos = { pos.x, Bottom() - scoreBox->Half().y };
}

void InfoBox::SetPos(Vector2 pos, Vector2 size)
{
	this->size = size;
	SetPos(pos);
}

void InfoBox::RenderOneLine(HDC hdc, string str, Vector2 startPos, int& cnt)
{
	TextOutA(hdc, (int)startPos.x, (int)startPos.y + FONT_HEIGHT * (cnt++), str.c_str(), (int)str.size());
}

void InfoBox::RenderText(HDC hdc)
{
	auto postColor = SetTextColor(hdc, YELLOW);

	//문자열 시작 영역
	Vector2 strStartPos = { showImage->pos.x - 100.0f, showImage->Bottom() + 5.0f };
	int cntLine = 0;

	string str = dataInfo->name;
	RenderOneLine(hdc, str, strStartPos, cntLine);

	str = "비용 : " + to_string(dataInfo->cost);
	RenderOneLine(hdc, str, strStartPos, cntLine);

	cntLine++;
	for (int i = 0; i < dataInfo->text.size(); i++) {
		str = dataInfo->text[i];
		int spos = 0;
		while (spos < str.size()) {
			int next = spos;
			while (spos + TEXT_SIZE > next) {
				next = (int)str.find(' ', next);
				if (next == string::npos) {
					next = (int)str.size();
					break;
				}
				else
					next++;
			}

			string buf = str.substr(spos, max(0, next - spos));
			RenderOneLine(hdc, buf, strStartPos, cntLine);
			spos = next;
		}
	}

	//텍스트 컬러 원상복구
	SetTextColor(hdc, postColor);
}


void InfoBox::RenderScore(HDC hdc)
{
	scoreBox->Render(hdc);

	Vector2 strStartPos = { scoreBox->pos.x-60.0f, scoreBox->pos.y-40.0f };
	Vector2 strEndPos = { scoreBox->pos.x+60.0f , scoreBox->pos.y-40.0f };

	auto& players = DominionGameMaster::Get()->players;
	int cnt = 0;
	RenderOneLine(hdc, "Score", {scoreBox->pos.x - 15.0f, strStartPos.y}, cnt);

	string str;
	for (auto player : players) {
		str = player->name;
		TextOutA(hdc, (int)strStartPos.x, (int)strStartPos.y + FONT_HEIGHT * (cnt), str.c_str(), (int)str.size());
	
		str = to_string(player->GetScore());
		TextOutA(hdc, (int)strEndPos.x - 6 * ((int)str.size()-1), (int)strStartPos.y + 20 * (cnt++), str.c_str(), (int)str.size());
	}
}
