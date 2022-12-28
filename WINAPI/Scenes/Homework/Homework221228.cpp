#include "framework.h"
#include "Homework221228.h"

Homework221228::Homework221228()
{
	land = new ImageRect(L"Textures/1228/Map.bmp", 1, 1, MAGENTA);
	land->pos = land->Half();
	
	Texture* expTexture = Texture::Add(L"Textures/Effect/explosion.bmp", 4, 4);
	EffectManager::Get()->Add("Exp", 30, expTexture, 3.0f);

	Camera::Get()->SetMapRect(land);

	player = new Player1228;
	player->pos = { WIN_WIDTH * 0.2f, 200.0f };
	player->SetLand(land->GetTexture());

	Camera::Get()->SetTarget(player);

	monster = new Monster1228;
	monster->pos = { land->size.x * 0.25f, land->size.y * 0.5f };
	monster->SetLand(land->GetTexture());

	function<void(float, float)> func = bind(&Homework221228::Boom, this, placeholders::_1, placeholders::_2);
	BombManager1228::Get()->SetLand(land->GetTexture());
	BombManager1228::Get()->SetFunc(func);

	hBrush = CreateSolidBrush(MAGENTA);
	hPen = CreatePen(PS_SOLID, 10, MAGENTA);

	goal = new ImageRect(L"Textures/CookieRun/Coin.bmp", 6, 1);
	goal->pos = {380, 1850};
	goal->isActive = true;
}

Homework221228::~Homework221228()
{
	delete land;

	DeleteObject(hBrush);
	DeleteObject(hPen);

	delete player;
	delete monster;
	BombManager1228::Delete();
	delete goal;
}

void Homework221228::Init()
{
}

void Homework221228::Update()
{
	player->Update();
	monster->Update();
	BombManager1228::Get()->Update();
}

void Homework221228::Render(HDC hdc)
{
	land->CamRender(hdc);
	monster->Render(hdc);
	player->Render(hdc);
	wstring str = to_wstring(player->pos.x) + L"," + to_wstring(player->pos.y);
	TextOut(hdc, 50, 50, str.c_str(), str.size());

	goal->CamRender(hdc);
	BombManager1228::Get()->Render(hdc);
}

void Homework221228::Boom(float x, float y)
{
	EffectManager::Get()->Play("Exp", { x, y });

	SelectObject(land->GetTexture()->GetMemDC(), hBrush);
	SelectObject(land->GetTexture()->GetMemDC(), hPen);

	int left = (int)(x - expRange);
	int right = (int)(x + expRange);
	int top = (int)(y - expRange);
	int bottom = (int)(y + expRange);
	Ellipse(land->GetTexture()->GetMemDC(), left, top, right, bottom);
}

///////////////////////////////////////////////////////////////////////////

Character1228::Character1228(wstring file, UINT frameX, UINT frameY, COLORREF transColor)
	: ImageRect(file, frameX, frameY, transColor)
{

	bodyRect = new Rect(pos, size);
}

Character1228::~Character1228()
{
	for (pair<ActionType, Animation*> animation : animations) {
		delete animation.second;
	}
	delete bodyRect;
}

void Character1228::Update()
{
	Move();
	Jump();
	SetAnimation();

	bodyRect->pos = pos + bodyOffset;

	animations[curType]->Update();

}

void Character1228::Render(HDC hdc)
{
	CamRender(hdc, animations[curType]->GetFrame());
}

bool Character1228::LandCollisionX()
{
	bool blocked = false;
	if (velocity.x < 0.0f) {
		//¿ÞÂÊ
		Vector2 LeftPos = { bodyRect->Left(), bodyRect->pos.y };
		blocked = landTexture->GetPixelHeight(LeftPos) <= bodyRect->pos.y;
	}
	else {
		//¿À¸¥ÂÊ
		Vector2 RightPos = { bodyRect->Right(), bodyRect->pos.y };
		blocked = landTexture->GetPixelHeight(RightPos) <= bodyRect->pos.y;
	}

	if (blocked) {
		pos.x -= velocity.x * DELTA;
		bodyRect->pos.x = pos.x + bodyOffset.x;
		velocity.x = 0.0f;
	}
	return blocked;
}

bool Character1228::LandCollisionY()
{
	bool blocked = false;
	float height = 0.0f;

	if (velocity.y < 0.0f) {
		Vector2 bottomPos = bodyRect->pos;
		bottomPos.y = bodyRect->Bottom() - 20.0f;
		height = landTexture->GetPixelHeight(bottomPos);

		if (bodyRect->Bottom() > height) {
			blocked = true;
			pos.y = height - Half().y;
			bodyRect->pos.y = pos.y + bodyOffset.y;
			velocity.y = 0.0f;
			jumpCount = 0;
		}
	}
	else if (velocity.y > 0.0f) {

		Vector2 topPos = bodyRect->pos;
		topPos.y = bodyRect->Top();
		height = landTexture->GetPixelHeight(topPos);

		if (bodyRect->Top() > height) {
			blocked = true;
			bodyRect->pos.y = height + bodyRect->Half().y;
			pos.y = bodyRect->pos.y - bodyOffset.y;
			velocity.y = 0.0f;
		}
	}

	return blocked;
}


void Character1228::Move()
{
	bool isMove = false;
	isMove |= abs(velocity.x) > 1.0f;
	if (!isMove) {
		velocity.x = 0.0f;
	}

	pos.x += velocity.x * DELTA;
	bodyRect->pos = pos + bodyOffset;

	xCollision = LandCollisionX();
}

void Character1228::Jump()
{

	velocity.y -= GRAVITY * DELTA;
	pos.y -= velocity.y * DELTA;
	bodyRect->pos = pos + bodyOffset;

	if (LandCollisionY())
		jumpCount = 0;
}


void Character1228::SetAnimation()
{
	if (velocity.y < -1.0f) {
		SetAction(ActionType::JUMP_DOWN);
		return;
	}
	else if (velocity.y > 1.0f) {
		SetAction(ActionType::JUMP_UP);
		return;
	}

	if (abs(velocity.x) > 0.0f)
		SetAction(ActionType::RUN);
	else
		SetAction(ActionType::IDLE);
}

void Character1228::SetAction(ActionType type)
{
	if (curType == type)
		return;

	curType = type;
	animations[curType]->Play();
}


////////////////////////////////////////////////////////////////////////////////

Bomb1228::Bomb1228() 
	: Character1228(L"Textures/1226/Monster.bmp", 4, 4)
{

	CreateAnimations();
	animations[curType]->Play();
}

Bomb1228::~Bomb1228()
{
}

void Bomb1228::Update()
{
	__super::Move();
	__super::Jump();

	lifeTime -= DELTA;
	if (lifeTime <= 0.0f) {
		if (boomFunc)
			boomFunc(pos.x, pos.y);

		isActive = false;
	}
}

void Bomb1228::Spawn(Vector2 pos)
{
	isActive = true;
	this->pos = pos;
	lifeTime = LIFETIME;
}


void Bomb1228::CreateAnimations()
{
	animations[ActionType::IDLE] = new Animation(texture->GetFrame());
	animations[ActionType::IDLE]->SetPart(0, 2);

	animations[ActionType::RUN] = new Animation(texture->GetFrame());
	animations[ActionType::RUN]->SetPart(0, 2);

	animations[ActionType::JUMP_UP] = new Animation(texture->GetFrame());
	animations[ActionType::JUMP_UP]->SetPart(0, 2, false);

	animations[ActionType::JUMP_DOWN] = new Animation(texture->GetFrame());
	animations[ActionType::JUMP_DOWN]->SetPart(0, 2, false);
}

/// <summary>
/// /////////////////////////////////////////////////////////////////////////
/// </summary>

const string BombManager1228::KEY = "Bomb";

BombManager1228::BombManager1228() {
	CreateObjects(KEY, 50);

}

void BombManager1228::CreateObjects(string key, UINT poolSize)
{
	vector<GameObject*> objects(poolSize);
	for (auto& object : objects) {
		object = new Bomb1228();
		object->isActive = false;
	}

	totalObjects[KEY] = objects;
}

void BombManager1228::SetFunc(function<void(float, float)>& func)
{
	auto objects = totalObjects[KEY];
	for (auto object : objects) {
		Bomb1228* bomb = (Bomb1228*)object;
		bomb->SetBoom(func);
	}
}

void BombManager1228::Spawn(Vector2 pos)
{
	Bomb1228* bomb = (Bomb1228*)Pop(KEY);
	if (bomb == nullptr)
		return;

	bomb->Spawn(pos);
}

void BombManager1228::SetLand(Texture* texture)
{

	auto objects = totalObjects[KEY];
	for (auto object : objects) {

		Bomb1228* bomb = (Bomb1228*)object;
		bomb->SetLand(texture);
	}
}

Player1228::Player1228()
	: Character1228(L"Textures/CookieRun.bmp", 11, 6)
{

	CreateAnimations();
	animations[curType]->Play();

	bodyOffset = { 0,120 };
	bodyRect->pos = pos + bodyOffset;
	bodyRect->size = { 95, 133 };
}

Player1228::~Player1228()
{
}

void Player1228::Update()
{
	Control();
	__super::Update();
}


void Player1228::Control() {
	velocity.x = 0.0f;
	if (KEY_PRESS(VK_RIGHT)) {
		velocity.x = speed;
	}
	if (KEY_PRESS(VK_LEFT)) {
		velocity.x = -speed;
	}

	if (jumpCount < 2 && KEY_DOWN(VK_UP)) {
		velocity.y = JUMP_POWER;
		jumpCount++;

		if (jumpCount >= 2)
		{
			SetAction(ActionType::JUMP_DOWN);
		}
	}

	if (KEY_DOWN(VK_SPACE)) {
		BombManager1228::Get()->Spawn(pos);
	}
}


void Player1228::CreateAnimations() {
	animations[ActionType::IDLE] = new Animation(texture->GetFrame());
	animations[ActionType::IDLE]->SetPart(35, 37);

	animations[ActionType::RUN] = new Animation(texture->GetFrame());
	animations[ActionType::RUN]->SetPart(11, 18);

	animations[ActionType::JUMP_UP] = new Animation(texture->GetFrame());
	animations[ActionType::JUMP_UP]->SetPart(0, 0, false);

	animations[ActionType::JUMP_DOWN] = new Animation(texture->GetFrame());
	animations[ActionType::JUMP_DOWN]->SetPart(1, 1, false);
}

Monster1228::Monster1228()
	: Character1228(L"Textures/1228/Yeti.bmp", 10, 2)
{
	CreateAnimations();
	animations[curType]->Play();
}

Monster1228::~Monster1228()
{
}

void Monster1228::Update()
{
	Control();
	__super::Update();

	if (xCollision)
		dest *= -1.0f;
}

void Monster1228::CreateAnimations()
{
	int arr1[] = { 0, 0, 0 };
	animations[ActionType::IDLE] = new Animation(texture->GetFrame());
	animations[ActionType::IDLE]->SetArray(arr1, 3, true);

	animations[ActionType::RUN] = new Animation(texture->GetFrame());
	animations[ActionType::RUN]->SetPart(1, 8);

	animations[ActionType::JUMP_UP] = new Animation(texture->GetFrame());
	animations[ActionType::JUMP_UP]->SetPart(11, 12, false);

	animations[ActionType::JUMP_DOWN] = new Animation(texture->GetFrame());
	animations[ActionType::JUMP_DOWN]->SetPart(13, 14, false);
}

void Monster1228::Control()
{
	velocity.x = dest * speed;
}
