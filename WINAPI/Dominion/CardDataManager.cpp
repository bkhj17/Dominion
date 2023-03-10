#include "framework.h"
#include "Card.h"
#include "CardDataManager.h"
#include "CardSet.h"
#include "DominionPlayer.h"

CardDataManager::CardDataManager()
{
	LoadData();
}

const CardData* CardDataManager::GetData(int key)
{
	//키 값에 맞는 데이터가 없다
	if (datas.find(key) == datas.end())
		return nullptr;

	//있으면 포인터 반환
	return &datas[key];
}

void CardDataManager::RenderCovered(HDC hdc, Rect* cardRect)
{
	texture->Render(hdc, cardRect);
}

void CardDataManager::RenderCovered(HDC hdc, Rect* cardRect, int alpha)
{
	texture->Render(hdc, cardRect, alpha);
}

void CardDataManager::LoadData()
{
	texture = Texture::Add(L"Textures/Dominion/Texture/CardTexture.bmp", 9, 4);

	ifstream ifs;
	ifs.open("Textures/Dominion/DominionData.csv");
	if (ifs.fail()) {
		return;
	}

	char buffer[3000];
	string str;
	str.reserve(3000);
	ifs.getline(buffer, sizeof(buffer), '\n');

	while (!ifs.eof()) {
		str = "";
		ifs.getline(buffer, sizeof(buffer), '\n');
		
		str = buffer;
		if (str.empty())
			continue;

		vector<string> vs = SplitString(str, ",");
		CardData data;
		data.key = stoi(vs[0]);
		data.name = vs[1];

		data.type.resize((int)CardType::END);
		vector<string> types = SplitString(vs[2], "/");
		for (string& t : types) {
			CardType ct = CardType::END;
			if (t == "재물")			ct = CardType::TREASURE;
			else if (t == "승점")	ct = CardType::VICTORY;
			else if (t == "저주")	ct = CardType::CURSE;
			else if (t == "액션")	ct = CardType::ACTION;
			else if (t == "공격")	ct = CardType::ATTACK;
			else if (t == "반응")	ct = CardType::REACT;
			data.type[(int)ct] = true;
		}

		data.cost = stoi(vs[3]);
		data.text = SplitString(vs[4], "/");
		for (auto& t : data.text) {
			for (auto& c : t) {
				if (c == '_')
					c = ' ';
			}
		}

		data.victory = stoi(vs[5]);
		data.sVictory = vs[6] == "1";
		data.texture = texture;
		data.covered = { 0, 0 };
		data.frame = { (data.key+1) % 9, (data.key+1) / 9};
		datas[data.key] = data;
	}

	sVictoryFunc[(int)CardKey::GARDENS] = bind(&SpecialVictory::GardenVictory, placeholders::_1);
}

int SpecialVictory::GardenVictory(void* player)
{
	auto p = (DominionPlayer*)player;
	if (p == nullptr)
		return 0;

	int count = 0;
	count += (int)p->deck->cards.size();
	count += (int)p->hand->cards.size();
	count += (int)p->used->cards.size();
	count += (int)p->discard->cards.size();

	return count / 10;
}
