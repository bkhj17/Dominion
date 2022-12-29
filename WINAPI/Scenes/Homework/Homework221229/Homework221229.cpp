#include "framework.h"
#include "Homework221229.h"

Homework221229::Homework221229()
{
	DataManager1229::Get();
	Inventory1229::Get();

	Texture* normal = Texture::Add(L"Textures/UI/NormalButton.bmp");
	Texture* over = Texture::Add(L"Textures/UI/OverButton.bmp");
	Texture* down = Texture::Add(L"Textures/UI/DownButton.bmp");

	shopButton = new Button(normal);
	shopButton->SetOverTexture(over);
	shopButton->SetDownTexture(down);
	shopButton->SetText("사기");
	shopButton->pos = { CENTER_X, CENTER_Y - 100.0f };
	shopButton->SetEvent([]() -> void {
		SceneManager::Get()->ChangeScene("Shop1229");
	});

	sellSceneButton = new Button(normal);
	sellSceneButton->SetOverTexture(over);
	sellSceneButton->SetDownTexture(down);
	sellSceneButton->SetText("팔기");
	sellSceneButton->pos = { CENTER_X, CENTER_Y };
	sellSceneButton->SetEvent([]() -> void {
		SceneManager::Get()->ChangeScene("Sell1229");
	});


	inventoryButton = new Button(normal);
	inventoryButton->SetOverTexture(over);
	inventoryButton->SetDownTexture(down);
	inventoryButton->SetText("인벤토리");
	inventoryButton->pos = { CENTER_X, CENTER_Y + 100.0f };
	inventoryButton->SetEvent([]() -> void {
		SceneManager::Get()->ChangeScene("Inventory1229");
		});


}

Homework221229::~Homework221229()
{
	DataManager1229::Delete();
	delete shopButton;
	delete sellSceneButton;
	delete inventoryButton;
}

void Homework221229::Update()
{
	shopButton->Update();
	sellSceneButton->Update();
	inventoryButton->Update();
}

void Homework221229::Render(HDC hdc)
{
	shopButton->Render(hdc);
	sellSceneButton->Render(hdc);
	inventoryButton->Render(hdc);
}
