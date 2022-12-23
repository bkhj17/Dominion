#include "framework.h"
#include "HomeworkScene221222.h"

const float HomeworkScene221222::SPEED = 100.0f;

HomeworkScene221222::HomeworkScene221222()
{
	HDC hdc = GetDC(hWnd);
	memDC = CreateCompatibleDC(hdc);				//hdc 크기의 memDC 생성
	ReleaseDC(hWnd, hdc);	//참조 종료


	PipeManager1222::Get();
	ScoreManager::Get();
	bird = new Bird1222;

	hBitmap = (HBITMAP)LoadImage(
		hInst,
		L"Textures/FlaffyBird.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE
	);

	bg = new LoopTexture1222(memDC, 0, 0, 143, 255);
	bird->SetTexture(new Texture1222(memDC, 264, 64, 17, 12));

	PipeManager1222::Get()->SetTopTexture(new Texture1222(memDC, 303, 0, 24, 125));
	PipeManager1222::Get()->SetBottomTexture(new Texture1222(memDC, 331, 12, 24, 109));
	PipeManager1222::Get()->SetTopHeadTexture(new Texture1222(memDC, 302, 123, 26, 12));
	PipeManager1222::Get()->SetBottomHeadTexture(new Texture1222(memDC, 330, 0, 26, 12));
}

HomeworkScene221222::~HomeworkScene221222()
{
	delete bird;
	PipeManager1222::Delete();
	DeleteObject(hBitmap);
}

void HomeworkScene221222::Update()
{
	if (gameOver) {
		if (KEY_DOWN(VK_SPACE)) {
			PostQuitMessage(0);
			return;
		}
	}

	PipeManager1222::Get()->Update();
	
	bird->Update();
	
	if (bird->IsFall() || PipeManager1222::Get()->Collision(bird)) {
		Timer::Get()->SetDeltaScale(0.0f);
		gameOver = true;
	}

	bg->Update();
}

void HomeworkScene221222::Render(HDC hdc)
{
	SelectObject(memDC, hBitmap);

	bg->Render(hdc, { 0, 0 }, {WIN_WIDTH, WIN_HEIGHT});

	bird->Render(hdc);
	PipeManager1222::Get()->Render(hdc);

	if (gameOver) {
		wstring str = L"Game Over. Score : " + to_wstring(ScoreManager::Get()->GetScore());
		TextOut(hdc, (int)(CENTER_X - 5 * str.size()), (int)CENTER_Y - 10, str.c_str(), (int)str.size());
	} 
	else
		ScoreManager::Get()->Render(hdc);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void HomeworkScene221222::Texture1222::Render(HDC hdc, Vector2 pos, Vector2 size) {
	if (memDC != nullptr) {
		GdiTransparentBlt(
			hdc,				//출력할 영역
			(int)(pos.x - size.x * 0.5f),
			(int)(pos.y - size.y * 0.5f),	//출력 위치
			(int)size.x, (int)size.y,	//출력 크기
			memDC,				//복사해올 영역
			copyX, copyY,				//복사위치
			sizeX, sizeY,	//복사할 크기
			MAGENTA				//무시할 색(배경)
		);
	}
}

void HomeworkScene221222::LoopTexture1222::Update() {
	moveX -= DELTA * xSpeed;
	if (moveX < 0.0f)
		moveX = 1.0f;
}

void HomeworkScene221222::LoopTexture1222::Render(HDC hdc, Vector2 pos, Vector2 size)
{

	if (memDC != nullptr) {
		GdiTransparentBlt(
			hdc,				//출력할 영역
			(int)pos.x,
			(int)pos.y,	//출력 위치
			(int)(size.x * moveX),
			(int)size.y,	//출력 크기
			memDC,				//복사해올 영역
			copyX + (int)((1.0f - moveX)*sizeX),
			copyY,				//복사위치
			(int)(sizeX * moveX), 
			sizeY,	//복사할 크기
			MAGENTA				//무시할 색(배경)
		);
		GdiTransparentBlt(
			hdc,				//출력할 영역
			(int)(pos.x + size.x * moveX),
			(int)pos.y,	//출력 위치
			(int)(size.x * (1.0f - moveX)), 
			(int)size.y,	//출력 크기
			memDC,				//복사해올 영역
			copyX, 
			copyY,				//복사위치
			(int)(sizeX * (1.0f - moveX)), 
			sizeY,	//복사할 크기
			MAGENTA				//무시할 색(배경)
		);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////


HomeworkScene221222::Bird1222::Bird1222()
	: Circle({ 100.0f, CENTER_Y }, 25.0f)
{
}

HomeworkScene221222::Bird1222::~Bird1222()
{
	delete texture;
}

void HomeworkScene221222::Bird1222::Update()
{
	speed += gravity * DELTA;
	if (KEY_DOWN(VK_SPACE)) {
		speed = -gravity;
	}

	pos.y += speed * DELTA;
	if (pos.y < radius) {
		pos.y = radius;
		speed = abs(speed);
	}
}

void HomeworkScene221222::Bird1222::Render(HDC hdc)
{
	if (texture != nullptr)
		texture->Render(hdc, pos, { radius * 2.0f, radius * 2.0f });
	else {
		__super::Render(hdc);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Vector2 HomeworkScene221222::Pipe1222::DEFAULT_SIZE = { 100.0f,300.0f };

HomeworkScene221222::Pipe1222::Pipe1222(Vector2 safePos, Vector2 size)
	: Rect({ safePos.x, CENTER_Y }, { size.x, WIN_HEIGHT })
	, safeArea(safePos.y, size.y)
{
}


void HomeworkScene221222::Pipe1222::Spawn()
{
	isActive = true;
	pass = false;
	pos = { WIN_WIDTH + size.x, pos.y };
	safeArea = { GameMath::Random(200.0f, WIN_HEIGHT - 200.0f), safeArea.y };
	SetPipe();
}

void HomeworkScene221222::Pipe1222::SetPipe()
{
	float topBottom = safeArea.x - safeArea.y * 0.5f;
	topRect.pos = Vector2(pos.x, (Top() + topBottom) * 0.5f);
	topRect.size = Vector2(size.x, abs(topBottom - Top()));

	float bottomTop = safeArea.x + safeArea.y * 0.5f;
	bottomRect.pos = Vector2(pos.x, (bottomTop + Bottom()) * 0.5f);
	bottomRect.size = Vector2(size.x, abs(Bottom() - bottomTop));
}


void HomeworkScene221222::Pipe1222::Update()
{
	if (!isActive)
		return;

	pos.x -= SPEED * DELTA;
	topRect.pos.x -= SPEED * DELTA;
	bottomRect.pos.x -= SPEED * DELTA;

	if (Right() < 0.0f)
		isActive = false;
}

void HomeworkScene221222::Pipe1222::Render(HDC hdc)
{
	if (!isActive)
		return;

	//__super::Render(hdc);
	topRect.Render(hdc);
	bottomRect.Render(hdc);
}

void HomeworkScene221222::Pipe1222::RenderTexture(
	HDC hdc,
	HomeworkScene221222::Texture1222* topHeadTexture,
	HomeworkScene221222::Texture1222* bottomHeadTexture,
	HomeworkScene221222::Texture1222* topTexture, 
	HomeworkScene221222::Texture1222* bottomTexture) {
	if (!isActive)
		return;

	if (topTexture != nullptr) {
		topTexture->Render(hdc, topRect.pos, topRect.size);
	}
	else {
		topRect.Render(hdc);
	}
	
	if (bottomTexture != nullptr) {
		bottomTexture->Render(hdc, bottomRect.pos, bottomRect.size);
	}
	else {
		bottomRect.Render(hdc);
	}

	if (topHeadTexture != nullptr) {
		topHeadTexture->Render(hdc,
			{ pos.x, topRect.Bottom() - 30.0f },
			{ size.x + 5.0f, 60.0f });
	}
	if(bottomHeadTexture != nullptr) {
		bottomHeadTexture->Render(hdc,
			{ pos.x, bottomRect.Top() + 30.0f },
			{ size.x + 5.0f, 60.0f });
	}
}


bool HomeworkScene221222::Pipe1222::Collision(Circle* circle) {
	if (!isActive || !circle->isActive)
		return false;

	if (!IsCircleCollision(circle)) {
		if (pass) {
			ScoreManager::Get()->ScoreUp();
			pass = false;
		}
		return false;
	}

	if (topRect.IsCircleCollision(circle))
		return true;
	
	if (bottomRect.IsCircleCollision(circle))
		return true;
	
	if (!pass) {
		pass = true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

string HomeworkScene221222::PipeManager1222::PIPE_KEY = "Pipe";

HomeworkScene221222::PipeManager1222::PipeManager1222()
{
	CreateObjects(PipeManager1222::PIPE_KEY, 5);
}

HomeworkScene221222::PipeManager1222::~PipeManager1222() {
	delete topTexture;
	delete bottomTexture;
	delete topHeadTexture;
	delete bottomHeadTexture;
}

void HomeworkScene221222::PipeManager1222::CreateObjects(string key, UINT poolSize)
{
	vector<GameObject*> objects(poolSize);
	for (auto& object : objects) {
		object = new Pipe1222();
		object->isActive = false;
	}

	totalObjects[key] = objects;
}

void HomeworkScene221222::PipeManager1222::Update()
{
	__super::Update();
	SpawnObject();
}

void HomeworkScene221222::PipeManager1222::Render(HDC hdc) {
	vector<GameObject*> objects = totalObjects[PIPE_KEY];
	for (auto object : objects) {
		if (!object->isActive)
			continue;

		Pipe1222* pipe = (Pipe1222*)object;
		pipe->RenderTexture(hdc, topHeadTexture, bottomHeadTexture, topTexture, bottomTexture);
	}
}

void HomeworkScene221222::PipeManager1222::SpawnObject()
{
	spawnTime += DELTA;
	if (spawnTime < spawnRate)
		return;

	vector<GameObject*> objects = totalObjects[PIPE_KEY];
	for (auto object : objects) {
		if (object->isActive)
			continue;
		Pipe1222* pipe = (Pipe1222*)object;
		pipe->Spawn();
		spawnTime = 0.0f;
		break;
	}
}

bool HomeworkScene221222::PipeManager1222::Collision(Circle* circle)
{
	if (!circle->isActive)
		return false;

	vector<GameObject*> objects = totalObjects[PIPE_KEY];
	for (auto object : objects) {
		if (!object->isActive)
			continue;
		Pipe1222* pipe = (Pipe1222*)object;
		if (pipe->Collision(circle))
			return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////

void HomeworkScene221222::ScoreManager::ScoreUp()
{
	score++;
}

void HomeworkScene221222::ScoreManager::Render(HDC hdc) {
	wstring str = L"Score : " + to_wstring(score);
	TextOut(hdc, WIN_WIDTH - 500, 0, str.c_str(), (int)str.length());
}
