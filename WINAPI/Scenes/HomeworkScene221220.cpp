#include "Framework.h"
#include "HomeworkScene221220.h"

const Vector2 HomeworkScene221220::Plane1220::DEFAULT_SIZE = { 70.0f, 50.0f };

HomeworkScene221220::HomeworkScene221220()
{
	plane = new Plane1220();
	plane->pos.x = WIN_WIDTH * 0.5f;
	plane->pos.y = WIN_HEIGHT - plane->GetSize().y *0.5f;

	bullets.resize(200);
	for (auto& bullet : bullets)
		bullet = new Bullet1220;

	monsters.resize(20);
	for (auto& monster : monsters)
		monster = new Monster();

	hPlaneBrush = CreateSolidBrush(RGB(100, 100, 250));
	hBulletBrush = CreateSolidBrush(RGB(255, 255, 0));
	hMonsterBrush = CreateSolidBrush(RGB(255, 0, 0));
}

HomeworkScene221220::~HomeworkScene221220()
{
	delete plane;

	for (auto bullet : bullets)
		delete bullet;

	for (auto monster : monsters)
		delete monster;

	DeleteObject(hPlaneBrush);
	DeleteObject(hBulletBrush);
	DeleteObject(hMonsterBrush);
}

void HomeworkScene221220::ShotBullet()
{
	if (plane->Shot()) {
		for (auto bullet : bullets) {
			if (!bullet->isActive) {
				bullet->Init(plane->ShotPos());
				break;
			}
		}
	}
}

void HomeworkScene221220::UpdateBullets()
{
	for (auto bullet : bullets) {
		bullet->Update();
		if (!bullet->isActive)
			continue;

		for (auto monster : monsters) {
			if (bullet->IsCollision(monster)) {
				monster->Destroy();
				bullet->Destroy();
				score++;
			}
		}
	}
}


void HomeworkScene221220::SpawnMonsters()
{
		int cnt = GameMath::Random(1, 4);
		for (auto monster : monsters) {
			if (!monster->isActive) {
				monster->Init();
				if (--cnt == 0)
					break;
			}
		}
}

void HomeworkScene221220::UpdateMonsters()
{

	for (auto monster : monsters) {
		monster->Update();
		if (monster->pos.y > WIN_HEIGHT) {
			//게임오버
			isPlaying = false;
		}
	}
}

void HomeworkScene221220::Init()
{
}

void HomeworkScene221220::Update()
{
	if (!isPlaying)
		return;

	plane->Update();
	if (KEY_PRESS(VK_SPACE) || KEY_DOWN(VK_SPACE))
		ShotBullet();
	UpdateBullets();

	monsterWait += DELTA;
	if (monsterWait >= monsterRate) {
		SpawnMonsters();
		monsterWait = 0.0f;
	}
	UpdateMonsters();
}

void HomeworkScene221220::Render(HDC hdc)
{
	SelectObject(hdc, hPlaneBrush);
	plane->Render(hdc);

	SelectObject(hdc, hBulletBrush);
	for (auto bullet : bullets) {
		bullet->Render(hdc);
	}

	SelectObject(hdc, hMonsterBrush);
	for (auto monster : monsters) {
		monster->Render(hdc);
	}

	if (!isPlaying) {
		wstring outText = L"Game Over...";

		TextOut(hdc,
			(int)(CENTER_X - outText.size() * 5),
			(int)(CENTER_Y - 10),
			outText.c_str(), (int)outText.size());
	}


	wstring str = L"Score : " + to_wstring(score);
	TextOut(hdc, WIN_WIDTH - 150, 0, str.c_str(), (int)str.size());
}

HomeworkScene221220::Plane1220::Plane1220(Vector2 pos)
	: Rect(pos, DEFAULT_SIZE)
{
	shotPos = Vector2(0, -size.y / 2);

	wing = Rect(-size.x * 0.5f, -size.y * 0.3f, size.x * 0.5f, size.y * 0.1f);
	body = Rect(-size.x * 0.2f, -size.y * 0.5f, size.x * 0.2f, size.y * 0.5f);
}

void HomeworkScene221220::Plane1220::Update()
{
	Move();

	shotWait += DELTA;
}

void HomeworkScene221220::Plane1220::Render(HDC hdc)
{
	if (!isActive)
		return;

	Rect renderBody = body;
	renderBody.pos += pos;

	Rect renderWing = wing;
	renderWing.pos += pos;

	Ellipse(hdc, 
		(int)renderBody.Left(), 
		(int)renderBody.Top(), 
		(int)renderBody.Right(), 
		(int)renderBody.Bottom());
	renderWing.Render(hdc);

}
void HomeworkScene221220::Plane1220::Move()
{
	Vector2 move(0.0f, 0.0f);
	if (KEY_DOWN(VK_LEFT) || KEY_PRESS(VK_LEFT))
		move -= {1, 0};
	if (KEY_DOWN(VK_RIGHT) || KEY_PRESS(VK_RIGHT))
		move += {1, 0};
	if (KEY_DOWN(VK_UP) || KEY_PRESS(VK_UP))
		move -= {0, 1};
	if (KEY_DOWN(VK_DOWN) || KEY_PRESS(VK_DOWN))
		move += {0, 1};
	move.Normalize();

	pos += move * moveSpeed * DELTA;
	if (pos.x < size.x * 0.5f)
		pos.x = size.x * 0.5f;
	if (pos.x > WIN_WIDTH - size.x * 0.5f)
		pos.x = WIN_WIDTH - size.x * 0.5f;
	if (pos.y < size.y * 0.5f)
		pos.y = size.y * 0.5f;
	if (pos.y > WIN_HEIGHT - size.y * 0.5f)
		pos.y = WIN_HEIGHT - size.y * 0.5f;
}

bool HomeworkScene221220::Plane1220::Shot() {
	if (shotWait < shotRate)
		return false;
	
	shotWait = 0.0f;
	return true;
}

const float HomeworkScene221220::Bullet1220::DEFUALT_RADIUS = 10.0f;
const float HomeworkScene221220::Bullet1220::DEFUALT_SPEED = 2000.0f;

HomeworkScene221220::Bullet1220::Bullet1220()
{
	radius = DEFUALT_RADIUS;
	isActive = false;
}

void HomeworkScene221220::Bullet1220::Destroy()
{
	isActive = false;
}

void HomeworkScene221220::Bullet1220::Init(Vector2 pos)
{
	isActive = true;
	this->pos = pos;
}

void HomeworkScene221220::Bullet1220::Update()
{
	if (!isActive)
		return;

	if (pos.y < 0) {
		isActive = false;
	}

	pos.y -= DELTA * speed;
}


const float HomeworkScene221220::Monster::MONSTER_SIZE_MIN = 50.0f;
const float HomeworkScene221220::Monster::MONSTER_SIZE_MAX = 100.0f;

HomeworkScene221220::Monster::Monster()
{
	isActive = false;
}

void HomeworkScene221220::Monster::Init()
{
	isActive = true;
	float sz = GameMath::Random(MONSTER_SIZE_MIN, MONSTER_SIZE_MAX);
	size.x = sz;
	size.y = sz;
	pos.x = GameMath::Random(size.x * 0.5f, WIN_WIDTH - size.y * 0.5f);
	pos.y = size.y * 0.5f;

	speed = GameMath::Random(10.0f, 100.0f);
}

void HomeworkScene221220::Monster::Update()
{
	if (!isActive)
		return;

	pos.y += speed * DELTA;
}

void HomeworkScene221220::Monster::Destroy() {
	isActive = false;
}