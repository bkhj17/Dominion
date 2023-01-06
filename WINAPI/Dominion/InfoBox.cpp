#include "framework.h"
#include "Card.h"
#include "CardDataManager.h"
#include "InfoBox.h"
#include "GameMaster.h"
#include "DominionPlayer.h"
#include "SelectWindow.h"

InfoBox::InfoBox()
	: Rect()
{
	image = new Rect(imagePos, { 200.0f, 300.0f });
}

InfoBox::~InfoBox()
{
	delete image;
}

void InfoBox::Init()
{
	dataInfo = nullptr;
}

void InfoBox::Update()
{
	image->pos = {pos.x, Top() + image->Half().y};

	CardData* mouseOn = nullptr;
	if (SelectWindow::Get()->isActive)
		mouseOn = SelectWindow::Get()->GetMouseOn();
	else 
		mouseOn = DominionGameMaster::Get()->GetMouseOn();

	if (mouseOn != nullptr) {
		SetData(mouseOn);
	}
}

void InfoBox::Render(HDC hdc)
{
	LineRender(hdc);

	if (dataInfo != nullptr) {
		dataInfo->texture->Render(hdc, image, dataInfo->frame);
		RenderText(hdc);
	}
	else
		CardDataManager::Get()->RenderCovered(hdc, image);

	RenderScore(hdc);
}

void InfoBox::RenderText(HDC hdc)
{
	Vector2 strStartPos = { image->pos.x, image->Bottom() + 5.0f };
	int cntLine = 0;

	string str = dataInfo->name;
	RenderOneLine(hdc, str, strStartPos, cntLine);

	str = "ºñ¿ë : " + to_string(dataInfo->cost);
	RenderOneLine(hdc, str, strStartPos, cntLine);

	cntLine++;
	for (int i = 0; i < dataInfo->text.size(); i++) {
		str = dataInfo->text[i];
		int spos = 0;
		while (spos < str.size()) {
			int next = spos;
			while (spos + 15 > next) {
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
}

void InfoBox::RenderOneLine(HDC hdc, string str, Vector2 startPos, int& cnt)
{
	TextOutA(hdc, (int)startPos.x - 4 * (int)str.size(), (int)startPos.y + 20 * (cnt++), str.c_str(), (int)str.size());
}

void InfoBox::RenderScore(HDC hdc)
{
	Vector2 strStartPos = { pos.x, Bottom() - 100.0f };
	
	auto players = DominionGameMaster::Get()->players;
	int cnt = 0;
	for (auto player : players) {
		string str = player->name + " : " + to_string(player->GetScore());
		RenderOneLine(hdc, str, strStartPos, cnt);
	}

}
