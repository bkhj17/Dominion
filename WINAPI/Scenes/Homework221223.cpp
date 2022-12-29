#include "Framework.h"
#include "Homework221223.h"


Stage221223::Stage221223()
{

	Texture* GoalTexture = Texture::Add(L"Textures/1223/Goal.bmp", 7, 1);

	goal = new Goal221223(GoalTexture);
	LoadStage("TextData/Homework221223/RunStage.csv");
}

Stage221223::~Stage221223()
{
	delete goal;
	for (auto dust : dusts)
		delete dust;
	dusts.clear();
	for (auto land : lands)
		delete land;
	lands.clear();
}

void Stage221223::LoadStage(string filename)
{
	Texture* landTexture = Texture::Add(L"Textures/1223/Land.bmp");
	Texture* dustTexture = Texture::Add(L"Textures/1223/Dust.bmp");

	ifstream ifs;
	ifs.open(filename);

	vector<string> vstr;
	while (!ifs.eof()) {
		string s;
		ifs >> s;
		if (s.empty())
			break;
		vstr.push_back(s);
	}

	float inverse = 1.0f / (int)vstr.size();
	const Vector2 gridSize = { SIZE_X, inverse * WIN_HEIGHT};
	int ypos = 0;
	for(auto& s : vstr) {
		vector<string> data = Utility::SplitString(s, ",");

		for (int i = 0; i < data.size(); i++) {
			Vector2 pos(gridSize.x * (i + 0.5f), gridSize.y * (ypos + 1.0f));
			switch (stoi(data[i]))
			{
			case 1:
			{
				ImageRect* land = new ImageRect(landTexture);
				land->pos = pos;
				lands.push_back(land);
			}
				break;
			case 2:
			{
				ImageRect* dust = new ImageRect(dustTexture);
				dust->pos = pos;
				dusts.push_back(dust);
			}
				break;
			case 3:
			{
				goal->pos = { pos.x, CENTER_Y };
			}
				break;
			}
		}
		ypos++;
	}

	lands.shrink_to_fit();
	dusts.shrink_to_fit();
}

void Stage221223::Update(Vector2 velocity)
{
	for (auto land : lands) {
		land->pos.x -= velocity.x;
	}

	for (auto dust : dusts) {
		dust->pos.x -= velocity.x;
	}

	goal->Update();
	goal->pos.x -= velocity.x;
}

void Stage221223::Render(HDC hdc, Rect* screen)
{
	if (!screen || screen == nullptr)
		return;

	if (goal->IsRectCollision(screen))
		goal->Render(hdc);

	for (auto land : lands) {
		if (land->isActive && land->IsRectCollision(screen))
			land->Render(hdc);
	}

	for (auto dust : dusts) {
		if (dust->isActive && dust->IsRectCollision(screen))
			dust->Render(hdc);
	}
}

void Stage221223::LandCollision(Rect* lane)
{
	for (auto land : lands) {
		Megaman* megaman = (Megaman*)lane;
		if (megaman->LandCollision(land))
			return;
	}
}

void Stage221223::DustCollision(Rect* lane)
{
	for (auto dust : dusts) {
		if (dust->isActive && dust->IsRectCollision(lane)) {
			dust->isActive = false;
			HomeworkScene221223::score++;
		}
	}
}

bool Stage221223::GoalCollision(Rect* lane)
{
	return goal->IsRectCollision(lane);
}


/// //////////////////////////////////////////////////////////////////////////////
int HomeworkScene221223::score = 0;

HomeworkScene221223::HomeworkScene221223()
{
	score = 0;

	megaman = new Megaman();
	megaman->pos = {WIN_WIDTH * 0.2f, CENTER_Y};

	stage = new Stage221223();

	screen = new Rect(0.0f, 0.0f, WIN_WIDTH, WIN_HEIGHT);

	landScape = new LandScape(L"Textures/CookieRun/Background.bmp");
}

HomeworkScene221223::~HomeworkScene221223()
{
	delete megaman;
	delete stage;
	delete landScape;
	delete screen;
}

void HomeworkScene221223::Init()
{
}

void HomeworkScene221223::Update()
{

	megaman->Update();
	stage->DustCollision(megaman->GetBody());
	stage->LandCollision(megaman);

	if (isPlaying && stage->GoalCollision(megaman->GetBody())) {
		isPlaying = false;
		
		wstring msg = L"목적지에 도달하였습니다. 모은 보석" + to_wstring(score);
		MessageBox(hWnd, msg.c_str(), L"Clear", MB_OK);
		Timer::Get()->SetDeltaScale(0.0f);
	}

	Vector2 velocity = megaman->GetVelocity();
	stage->Update(velocity);
	screen->pos.x += velocity.x * DELTA;
	
	landScape->SetSpeed(velocity.x * 2000.0f);
	landScape->Update();
	if (isPlaying && megaman->Top() > screen->Bottom()) {
		isPlaying = false;

		MessageBox(hWnd, L"떨어졌습니다.", L"Game Over", MB_OK);
		Timer::Get()->SetDeltaScale(0.0f);
	}

}

void HomeworkScene221223::Render(HDC hdc)
{
	landScape->Render(hdc);
	stage->Render(hdc, screen);
	megaman->Render(hdc);
}

Goal221223::Goal221223(Texture* texture)
	:ImageRect(texture)
{
	animation = new Animation(texture->GetFrame());
	animation->SetDefault();
	animation->Play();
}

Goal221223::~Goal221223()
{
	if(animation)
		delete animation;
}

void Goal221223::Update()
{
	animation->Update();
}

void Goal221223::Render(HDC hdc)
{
	ImageRect::Render(hdc, animation->GetFrame());
}
