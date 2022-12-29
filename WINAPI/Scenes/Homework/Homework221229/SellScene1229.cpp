#include "framework.h"
#include "SellScene1229.h"

SellScene1229::SellScene1229()
{
	normal = Texture::Add(L"Textures/UI/NormalButton.bmp");
	over = Texture::Add(L"Textures/UI/OverButton.bmp");
	down = Texture::Add(L"Textures/UI/DownButton.bmp");

	backButton = new Button(normal);
	backButton->SetOverTexture(over);
	backButton->SetDownTexture(down);
	backButton->pos = { CENTER_X, WIN_HEIGHT - backButton->Half().y };
	backButton->SetText("뒤로 가기");
	backButton->SetEvent([]() -> void {
		SceneManager::Get()->ChangeScene("Homwork1229");
	});
}

SellScene1229::~SellScene1229()
{
	End();
	delete backButton;
}

void SellScene1229::Start()
{
	int cnt = 0;
	for (auto item : Inventory1229::Get()->items) {
		SellButton1229 * button = new SellButton1229(normal);
		button->index = cnt;
		button->SetOverTexture(over);
		button->SetDownTexture(down);
		button->pos = { CENTER_X, button->Half().y + button->size.y * cnt++ };
		button->SetEvent([button]()->void {
			Inventory1229::Get()->SellItem(button->index);
		});
		sellButtons.push_back(button);
	}
}

void SellScene1229::End()
{
	for (auto button : sellButtons) {
		delete button;
	}
	sellButtons.clear();
	sellButtons.shrink_to_fit();
}

void SellScene1229::Update()
{
	auto& items = Inventory1229::Get()->items;
	for (int i = 0; i < items.size(); i++)
		sellButtons[i]->Update();

	backButton->Update();
}

void SellScene1229::Render(HDC hdc)
{
	auto& items = Inventory1229::Get()->items;
	for(int i = 0; i < items.size(); i++) {
		BagUsable1229* usable = (BagUsable1229 *)items[i];
		string s = usable->data->name + +" "
			+ to_string(usable->data->price) + " (" + to_string(usable->count) + "개)";

		sellButtons[i]->SetText(s);
		sellButtons[i]->Render(hdc);
	}

	wstring str = L"Money : " + to_wstring(Inventory1229::Get()->GetMoney());
	TextOut(hdc, 100, 200, str.c_str(), str.size());

	backButton->Render(hdc);
}

