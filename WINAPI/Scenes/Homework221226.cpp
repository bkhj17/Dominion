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
	if(isControl)
		ImageOffset = { 0.0f, -50.0f };
	else
		ImageOffset = { 0.0f, 0.0f };

	if(isControl)
		pos = { WIN_WIDTH * 0.2f, WIN_HEIGHT - Half().y - 50.0f };
	else
		pos = { WIN_WIDTH * 0.8f, WIN_HEIGHT - Half().y - 50.0f};


	curHp = maxHp;

	CreateAnimations();
	curAction = ActionType::IDLE;
	animations[curAction]->Play();
}

Player1226::~Player1226()
{
	delete imageRect;

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
	if (isDead) {

	}


	velocityX = 0.0f;
	if(isControl)
		Input();
	else
		AI();
	Move();
	SetAnimation();

	animations[curAction]->Update();
}

void Player1226::Render(HDC hdc)
{

	imageRect->Render(hdc, alpha, animations[curAction]->GetFrame());
	imageRect->LineRender(hdc);
	LineRender(hdc);

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
	if(abs(velocityX) > 0.5f) {
		pos.x += velocityX * DELTA * moveSpeed;
	}

	imageRect->pos = pos + ImageOffset;
}

void Player1226::Attack()
{
	if (isAttack)
		return;

	isAttack = true;
	SetAction(ActionType::ATTACK);
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
	if (curAction == type)
		return;

	curAction = type;
	animations[curAction]->Play();
}

void Player1226::SetAnimation()
{
	if (!animations[curAction]->IsPlay() && !isDead) {
		isAttack = false;
		isHit = false;

		SetAction(ActionType::IDLE);
		return;
	}


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
		animations[ActionType::ATTACK] = new Animation(texture->GetFrame(), 3.0f);
		animations[ActionType::HIT] = new Animation(texture->GetFrame(), 3.0f);
		animations[ActionType::DEAD] = new Animation(texture->GetFrame(), 3.0f);
		animations[ActionType::IDLE]->SetPart(0, 9);
		animations[ActionType::RUN]->SetPart(10, 19);
		animations[ActionType::ATTACK]->SetPart(20, 26, false);
		animations[ActionType::HIT]->SetPart(40, 45, false);
		animations[ActionType::DEAD]->SetPart(50, 59, false);
	}
	else {
		animations[ActionType::IDLE] = new Animation(texture->GetFrame());
		animations[ActionType::RUN] = new Animation(texture->GetFrame());
		animations[ActionType::ATTACK] = new Animation(texture->GetFrame(), 3.0f);
		animations[ActionType::HIT] = new Animation(texture->GetFrame(), 3.0f);
		animations[ActionType::DEAD] = new Animation(texture->GetFrame(), 3.0f);
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


/////////////////////////////////////////////////////////////////

Homework221226::Homework221226()
{
	player = new Player1226(true);
	player->SetDeadEvent(bind(&Homework221226::GameEnd, this));
	monster = new Player1226(false);
	monster->SetDeadEvent(bind(&Homework221226::GameEnd, this));

	monster->SetEnemy(player);

	bg = new ImageRect(L"Textures/background.bmp");
	bg->pos = { CENTER_X, CENTER_Y };
}

Homework221226::~Homework221226()
{
	delete bg;
	delete player;
	delete monster;
}

void Homework221226::Init()
{
}

void Homework221226::Update()
{
	player->Update();
	if (!player->IsDead()) {
		Vector2 overlap;
		if (player->IsRectCollision(monster, &overlap)) {
			player->pos.x -= overlap.x;
		}

		if (player->IsAttack()) {
			if (!monster->IsHit() && !monster->IsDead()) {
				Rect attackRect = player->GetAttackRect();
				if (monster->IsRectCollision(&attackRect, &overlap)) {
					monster->Hit(player->AttackPoint());
					monster->pos.x += 100.0f;
				}
			}
		}
	}

	if (player->Left() < 0) {
		player->pos.x = player->Half().x;
	}
	
	monster->Update();
	if (!monster->IsDead()) {
		if (monster->IsAttack()) {
			if (!player->IsHit() && !player->IsDead()) {
				Rect attackRect = monster->GetAttackRect();
				Vector2 overlap;
				if (player->IsRectCollision(&attackRect, &overlap)) {
					player->Hit(monster->AttackPoint());
					player->pos.x -= 100.0f;
				}
			}
		}

	}
	if (monster->Right() > WIN_WIDTH) {
		monster->pos.x = WIN_WIDTH - monster->Half().x;
	}
}

void Homework221226::Render(HDC hdc)
{
	bg->Render(hdc);
	player->Render(hdc);
	monster->Render(hdc);
}

void Homework221226::GameEnd()
{
	if (gameEnd)
		return;

	wstring str = (player->IsDead() ? L"게임 오버" : L"승리");
	MessageBox(hWnd, str.c_str(), str.c_str(), MB_OK);
}
