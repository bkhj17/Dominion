//Homework221227.cpp
#include "framework.h"
#include "Homework221227.h"

NodeManager::NodeManager()
{
	CreateObjects("Node", POOL_SIZE);
}

NodeManager::~NodeManager()
{
	for (auto& object : nodes)
		delete object;
	nodes.clear();
	nodes.shrink_to_fit();
}

void NodeManager::CreateObjects(string key, UINT poolSize)
{
	nodes.resize(poolSize);
	for (auto& object : nodes) {
		object = new Circle(Vector2(), radius);
		object->isActive = false;
	}
}

void NodeManager::Collision(Circle* circle)
{
	for (int i = head; i != tail; i = (++i) % nodes.size()) {
		if (!nodes[i]->isActive)
			continue;

		if (nodes[i]->IsCircleCollision(circle)) {
			float dist = (circle->pos - nodes[i]->pos).Length();
			//dist 값에 따라 바꿈
			if (dist < radius * 0.1f) {
				if (scoringFunc)
					scoringFunc(Rank1227::PERFECT);
			}
			else if (dist < radius * 0.8f) {
				if (scoringFunc)
					scoringFunc(Rank1227::GREAT);
			}
			else {
				if (scoringFunc)
					scoringFunc(Rank1227::GOOD);
			}

			nodes[i]->isActive = false;
			return;
		}
	}
}

void NodeManager::Spawn()
{
	spawnTime += DELTA;
	if (spawnTime >= spawnRate) {
		if (head == (tail + 1) % nodes.size())
			return;

		nodes[tail]->isActive = true;
		nodes[tail]->pos = { WIN_WIDTH + radius, CENTER_Y };
		tail = (++tail) % nodes.size();

		spawnTime = 0.0f;
		spawnRate = Random(0.5f, 1.5f);
	}
}

void NodeManager::Update()
{
	for (int i = head; i != tail; i = (++i) % nodes.size()) {
		if (nodes[i]->isActive) {
			nodes[i]->pos.x -= moveSpeed * DELTA;

			if (nodes[i]->pos.x < -radius) {
				nodes[i]->isActive = false;
				if (scoringFunc)
					scoringFunc(Rank1227::MISS);
			}
		}
	}

	while (head != tail && !nodes[head]->isActive) {
		head = (head + 1) % nodes.size();
	}

	Spawn();
}

void NodeManager::Render(HDC hdc)
{
	for (auto& object : nodes) {
		object->Render(hdc);
	}
}

/////////////////////////////////////////////////////////////

Drum1227::Drum1227()
{
	radius = RADIUS;
}

Drum1227::~Drum1227()
{
}

void Drum1227::Update()
{
	if (KEY_DOWN(VK_SPACE)) {
		NodeManager::Get()->Collision(this);
		Audio::Get()->Play("Hit");
	}
}

void Drum1227::Render(HDC hdc)
{
	__super::Render(hdc);
}

//////////////////////////////////////////////////////////////

EffectRect::EffectRect()
	: ImageRect(L"Textures/1227/Effect.bmp", 4, 2)
{
	CreateAnimations();
	isActive = false;
}

EffectRect::~EffectRect()
{
	for (auto& animation : animations)
		delete animation.second;
}

void EffectRect::Play(Rank1227 rank)
{
	curAnim = rank;

	switch (rank)
	{
	case Rank1227::PERFECT:
	case Rank1227::GREAT:
	case Rank1227::GOOD:
	{
		isActive = true;
		animations[curAnim]->Play();
	}
	break;
	default:
		//isActive = false;
		break;
	}
}

void EffectRect::Update()
{
	if (isActive)
		animations[curAnim]->Update();
}

void EffectRect::Render(HDC hdc)
{
	if (isActive) {
		POINT frame = animations[curAnim]->GetFrame();
		ImageRect::Render(hdc, 125, frame);
	}
}

void EffectRect::CreateAnimations()
{
	animations[Rank1227::PERFECT] = new Animation(texture->GetFrame());
	animations[Rank1227::PERFECT]->SetPart(2, 5, false);
	animations[Rank1227::PERFECT]->SetEndEvent(bind(&EffectRect::EndEffect, this));

	animations[Rank1227::GREAT] = new Animation(texture->GetFrame());
	animations[Rank1227::GREAT]->SetPart(0, 1, false);
	animations[Rank1227::GREAT]->SetEndEvent(bind(&EffectRect::EndEffect, this));

	animations[Rank1227::GOOD] = new Animation(texture->GetFrame());
	animations[Rank1227::GOOD]->SetPart(6, 7, false);
	animations[Rank1227::GOOD]->SetEndEvent(bind(&EffectRect::EndEffect, this));
}

//////////////////////////////////////////////////////////////////////////

Homework221227::Homework221227()
{
	NodeManager::Get()->SetMessageFunc(bind(&Homework221227::Scoring, this, placeholders::_1));

	Audio::Get()->Add("Hit", "Sounds/hit.wav", false);

	drum = new Drum1227;
	drum->pos = { drum->GetRadius() + 50.0f, CENTER_Y };

	effect = new EffectRect;
	effect->pos = drum->pos;

	bg = new ImageRect(L"Textures/background.bmp");
	bg->pos = { CENTER_X, CENTER_Y };

	lane = new Rect({ CENTER_X, CENTER_Y }, { WIN_WIDTH, 200.0f });
	hBrush = (HBRUSH)CreateSolidBrush(BLACK);
}

Homework221227::~Homework221227()
{
	NodeManager::Delete();
	delete drum;
	delete effect;
	delete bg;
	delete lane;
	DeleteObject(hBrush);
}

void Homework221227::Init()
{
}

void Homework221227::Update()
{
	NodeManager::Get()->Update();
	drum->Update();
	effect->Update();
}

void Homework221227::Render(HDC hdc)
{
	bg->Render(hdc);
	auto origin = SelectObject(hdc, hBrush);
	lane->Render(hdc);
	SelectObject(hdc, origin);

	drum->Render(hdc);
	NodeManager::Get()->Render(hdc);

	effect->Render(hdc);

	if (message.size() > 0)
		TextOut(hdc, 0, 150, message.c_str(), (int)message.size());
}

void Homework221227::Scoring(Rank1227 rank)
{
	switch (rank)
	{
	case Rank1227::PERFECT:
		message = L"Perfect";
		break;
	case Rank1227::GREAT:
		message = L"Great";
		break;
	case Rank1227::GOOD:
		message = L"Good";
		break;
	case Rank1227::MISS:
		message = L"Miss";
		break;
	}
	effect->Play(rank);
}
