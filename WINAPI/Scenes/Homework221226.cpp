#include "framework.h"
#include "Homework221226.h"


Player1226::Player1226(bool isControl)
	: isControl(isControl)
{
	if(isControl)
		texture = Texture::Add(L"Textures/1226/Player.bmp", 10, 6);
	else
		texture = Texture::Add(L"Textures/1226/Monster.bmp", 4, 4);

	imageRect = new ImageRect(texture);


	size = { 200.0f, 200.0f };
	if (isControl) {
		imageOffset = { 0.0f, -50.0f };
		attackRange = 200.0f;
	}
	else {
		imageOffset = { 0.0f, 0.0f };
		attackRange = 30.0f;
	}
	hpOffset = { 0, -Half().y };
	hpbar = new Rect(hpOffset, {size.x, 40.0f});

	curHp = maxHp;
	maxInverse = 1.0f / maxHp;
	hBrush = (HBRUSH*)CreateSolidBrush(RGB(255, 0, 0));

	if(isControl)
		SetPos({ WIN_WIDTH * 0.2f, WIN_HEIGHT - Half().y - 50.0f });

	CreateAnimations();
	curAction = ActionType::IDLE;
	animations[curAction]->Play();

}

Player1226::~Player1226()
{
	delete imageRect;
	delete hpbar;
	DeleteObject(hBrush);

	for (pair<ActionType, Animation*> animation : animations) {
		delete animation.second;
	}
}

void Player1226::SetEnemy(Player1226* enemy)
{
	this->enemy = enemy;
}

void Player1226::Update()
{
	velocityX = 0.0f;
	if(isControl)
		Input();
	else
		AI();
	Move();
	SetAnimation();
	UpdateHPBar();

	
	animations[curAction]->Update();

}

void Player1226::Render(HDC hdc)
{

	imageRect->Render(hdc, alpha, animations[curAction]->GetFrame());
	imageRect->LineRender(hdc);
	LineRender(hdc);

	RenderHPBar(hdc);

	if (isAttack) {
		GetAttackRect().LineRender(hdc);
	}
}

void Player1226::Input()
{
	velocityX = 0.0f;
	if (KEY_PRESS(VK_LEFT)) {
		velocityX -= 1.0f;
	}
	if (KEY_PRESS(VK_RIGHT)) {
		velocityX += 1.0f;
	}

	if (KEY_DOWN('X')) {
		Attack();
	}

	if (KEY_DOWN('Z')) {
		auto fire = AttackBox1226::Get();
		if (!fire->isActive) {
			fire->isActive = true;
			fire->pos = pos;
			fire->attack = attack;
			SetAction(ActionType::ATTACK);
		}
	}
}

void Player1226::AI()
{
	if (enemy == nullptr)
		return;

	if (isHit || isDead)
		return;

	Vector2 toEnemy = { enemy->Right() - Left(), enemy->pos.y - pos.y };
	if (toEnemy.Length() < attackRange) {
		Attack();
	} 
	else if (toEnemy.Length() < 500.0f) {
		velocityX = toEnemy.GetNormalized().x;
	}
}

void Player1226::Move()
{
	if (isDead)
		return;

	if(abs(velocityX) > 0.5f) {
		pos.x += velocityX * DELTA * moveSpeed;
	}

	if (isControl) {
		MonsterManager::Get()->Collision(this);

		if (Left() < 0) {
			pos.x = Half().x;
		}
	}
	else {
		Vector2 overlap;
		if (IsRectCollision(enemy, &overlap)) {
			pos.x = enemy->Right() + Half().x;
		}


		if (Right() > WIN_WIDTH) {
			pos.x = WIN_WIDTH - Half().x;
		}
	}


	imageRect->pos = pos + imageOffset;
}

void Player1226::Attack()
{
	if (isDead || isAttack)
		return;

	isAttack = true;
	SetAction(ActionType::ATTACK);

	auto atkRect = GetAttackRect();
	if (isControl) {
		auto monsters = MonsterManager::Get()->GetMonsters();

		for (auto m : monsters) {
			if (m->IsCollision(&atkRect)) {
				Player1226* monster = (Player1226*)m;
				monster->Hit(attack);
			}
		}
	}
	else {
		if (enemy->IsCollision(&atkRect)) {
			enemy->Hit(attack);
		}
	}
}

void Player1226::Hit(int atk)
{
	if (curAction == ActionType::HIT)
		return;

	curHp -= atk;
	if (curHp <= 0) {
		Dead();
	}
	else {
		isHit = true;
		SetAction(ActionType::HIT);
	}
}

void Player1226::Dead()
{
	isDead = true;
	SetAction(ActionType::DEAD);
}

void Player1226::SetAction(ActionType type)
{
	if (curAction == ActionType::DEAD)
		return;

	if (curAction == type && animations[curAction]->IsPlay())
		return;

	curAction = type;
	animations[curAction]->Play();
}

void Player1226::SetAnimation()
{
	if (curAction == ActionType::ATTACK 
		|| curAction == ActionType::HIT 
		|| curAction == ActionType::DEAD)
		return;

	if (abs(velocityX) > 0.5f) {
		SetAction(ActionType::RUN);
		return;
	}
	SetAction(ActionType::IDLE);
}

void Player1226::CreateAnimations()
{

	if (isControl) {
		animations[ActionType::IDLE] = new Animation(texture->GetFrame());
		animations[ActionType::RUN] = new Animation(texture->GetFrame());
		animations[ActionType::ATTACK] = new Animation(texture->GetFrame(), 5.0f);
		animations[ActionType::HIT] = new Animation(texture->GetFrame());
		animations[ActionType::DEAD] = new Animation(texture->GetFrame());
		animations[ActionType::IDLE]->SetPart(0, 9);
		animations[ActionType::RUN]->SetPart(10, 19);
		animations[ActionType::ATTACK]->SetPart(20, 26, false);
		animations[ActionType::HIT]->SetPart(40, 45, false);
		animations[ActionType::DEAD]->SetPart(50, 59, false);
	}
	else {
		animations[ActionType::IDLE] = new Animation(texture->GetFrame());
		animations[ActionType::RUN] = new Animation(texture->GetFrame());
		animations[ActionType::ATTACK] = new Animation(texture->GetFrame(),5.0f);
		animations[ActionType::HIT] = new Animation(texture->GetFrame());
		animations[ActionType::DEAD] = new Animation(texture->GetFrame());
		animations[ActionType::IDLE]->SetPart(0, 1);
		animations[ActionType::RUN]->SetPart(4, 6);
		animations[ActionType::ATTACK]->SetPart(4, 5, false);
		animations[ActionType::HIT]->SetPart(12, 13, false);
		animations[ActionType::DEAD]->SetPart(12, 14, false);
	}

	animations[ActionType::ATTACK]->SetEndEvent(bind(&Player1226::AttactEnd, this));
	animations[ActionType::HIT]->SetEndEvent(bind(&Player1226::HitEnd, this));
}

Rect Player1226::GetAttackRect()
{
	if (isControl) {
		Rect attackRect = {
			Right(),
			Top(),
			Right() + attackRange,
			Bottom() };
		return attackRect;
	}
	else {
		Rect attackRect = {
			Left() - attackRange,
			Top(),
			Left(),
			Bottom() };
		return attackRect;
	}
}

void Player1226::SetDeadEvent(function<void ()> func)
{
	animations[ActionType::DEAD]->SetEndEvent(func);
}

void Player1226::UpdateHPBar()
{
	hpbar->pos = pos + hpOffset;
}

void Player1226::RenderHPBar(HDC hdc)
{
	hpbar->Render(hdc);

	if (curHp <= 0.0f)
		return;

	auto origin = SelectObject(hdc, hBrush);
	float hplen = size.x * (float)curHp* maxInverse;
	Rect curHpRect(hpbar->Left(), hpbar->Top(), hpbar->Left() + hplen, hpbar->Bottom());
	curHpRect.Render(hdc);
	SelectObject(hdc, origin);
}

void Player1226::SetPos(Vector2 pos)
{
	this->pos = pos;
	hpbar->pos = pos + hpOffset;
	imageRect->pos = pos + imageOffset;
}


/////////////////////////////////////////////////////////////////

Homework221226::Homework221226()
{
	player = new Player1226(true);
	player->SetDeadEvent(bind(&Homework221226::GameEnd, this));

	MonsterManager::Get()->SetEnemy(player);

	bg = new ImageRect(L"Textures/background.bmp");
	bg->pos = { CENTER_X, CENTER_Y };


	AttackBox1226::Get();
}

Homework221226::~Homework221226()
{
	delete bg;
	delete player;

	AttackBox1226::Delete();
	MonsterManager::Delete();
}

void Homework221226::Init()
{
}

void Homework221226::Update()
{
	player->Update();
	MonsterManager::Get()->Update();

	if (MonsterManager::Get()->GetCount() == 0) {
		GameEnd();
	}

	AttackBox1226::Get()->Update();
}

void Homework221226::Render(HDC hdc)
{
	bg->Render(hdc);
	player->Render(hdc);
	MonsterManager::Get()->Render(hdc);

	AttackBox1226::Get()->Render(hdc);
}

void Homework221226::GameEnd()
{
	if (gameEnd)
		return;
	gameEnd = true;

	wstring str = (player->IsDead() ? L"게임 오버" : L"승리");
	MessageBox(hWnd, str.c_str(), str.c_str(), MB_OK);
}

///////////////////////////////////////////////////////////////////////

AttackBox1226::AttackBox1226()
{
	Texture* texture = Texture::Add(L"Textures/1226/Fire.bmp", 3, 1);
	image = new ImageRect(texture);

	animation = new Animation({ 0, 2 });

	isActive = false;
}

AttackBox1226::~AttackBox1226()
{
	delete image;
	delete animation;
}

void AttackBox1226::Update()
{
	if (!isActive)
		return;

	pos.x += speed * DELTA;
	image->pos = pos;
	if (Left() > WIN_WIDTH) {
		isActive = false;
	}

	Collision();
}

void AttackBox1226::Render(HDC hdc)
{
	if (!isActive)
		return;
	image->Render(hdc);
}

void AttackBox1226::Collision()
{
	auto monsters = MonsterManager::Get()->GetMonsters();
	for (auto object : monsters) {
		if (!object->isActive)
			continue;

		Player1226* monster = (Player1226*)object;
		if (!monster->IsDead() && monster->IsCollision(this)) {
			monster->Hit(attack);
		}
	}
}

MonsterManager::MonsterManager()
{
	CreateObjects("Monster", 3);
}

int MonsterManager::GetCount()
{
	int cnt = 0;
	for (auto object : totalObjects["Monster"]) {
		if (object->isActive) {
			Player1226* monster = (Player1226*)object;
			if (!monster->IsDead())
				cnt++;
		}
	}

	return cnt;
}

void MonsterManager::SetEnemy(Player1226* player)
{
	for (auto object : totalObjects["Monster"]) {
		Player1226* monster = (Player1226*)object;
		monster->SetEnemy(player);
	}
}

void MonsterManager::Collision(Rect* rect)
{
	float left = WIN_WIDTH;

	for (auto object : totalObjects["Monster"]) {
		if (!object->isActive)
			continue;

		Player1226* monster = (Player1226*)object;
		if (monster->IsDead())
			continue;

		left = min(monster->Left(), left);
	}

	rect->pos.x = min(left - rect->Half().x, rect->pos.x);
}

void MonsterManager::CreateObjects(string key, UINT poolSize)
{

	int count = 0;
	vector<GameObject*> objects(poolSize);
	for (auto& object : objects) {
		Player1226* monster = new Player1226(false);
		monster->SetPos({WIN_WIDTH * (0.9f - 0.05f * count++), WIN_HEIGHT - monster->Half().y - 50.0f});

		object = monster;
	}
	totalObjects["Monster"] = objects;
}
