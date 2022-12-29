#include "framework.h"
#include "DigGameScene.h"

DigGameScene::DigGameScene()
{
	background = new ImageRect(L"Textures/Bomb/sky.bmp");

}

DigGameScene::~DigGameScene()
{
	delete background;
	BombManager::Delete();
}

void DigGameScene::Start()
{
	land = new ImageRect(L"Textures/Bomb/Ground.bmp", 1, 1, MAGENTA);
	land->pos = land->Half();

	background->size = land->size;
	background->pos = land->Half();

	megaman = new Megaman();
	megaman->pos = { CENTER_X, 0 };
	megaman->SetLand(land->GetTexture());

	Camera::Get()->SetTarget(megaman);
	//Camera::Get()->SetOffset(0.0f, -300.0f);
	Camera::Get()->SetMapRect(land);
	Camera::Get()->SetPos(megaman->pos);

	BombManager::Get()->SetLand(land->GetTexture());

	siva = new Siva();
	siva->pos = { CENTER_X + 100, 0 };
	siva->SetLand(land->GetTexture());

	Audio::Get()->Stop("BG");
}

void DigGameScene::End()
{
	delete megaman;
	delete siva;
	BombManager::Get()->Clear();

	delete land;

	Audio::Get()->Stop("BG");
}

void DigGameScene::Update()
{

	megaman->Update();
	siva->Update();
	BombManager::Get()->Update();

	if (KEY_DOWN('1')) {
		SceneManager::Get()->ChangeScene("Battle");
		return;
	}
}

void DigGameScene::Render(HDC hdc)
{
	background->CamRender(hdc, { 0,0 }, false);
	land->CamRender(hdc);
	megaman->Render(hdc);
	siva->Render(hdc);
	BombManager::Get()->Render(hdc);
}
