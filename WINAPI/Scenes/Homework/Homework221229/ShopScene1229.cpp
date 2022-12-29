
#include "Framework.h"
#include "ShopScene1229.h"

ShopScene1229::ShopScene1229()
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

ShopScene1229::~ShopScene1229()
{
	End();
	delete backButton;
}

void ShopScene1229::Start()
{

	int cnt = 0;
	for (auto& item : DataManager1229::Get()->ItemTable()) {
		BuyButton* button = new BuyButton(normal);
		button->SetOverTexture(over);
		button->SetDownTexture(down);
		button->pos = { CENTER_X, button->Half().y + button->size.y * cnt++ };
		button->itemData = const_cast<ItemData1229*>(item.second);
		auto inven =  Inventory1229::Get();
		function<bool(ItemData1229*)> event = bind(&Inventory1229::BuyItem, inven, placeholders::_1);
		button->SetEvent([button]() -> void {
			Inventory1229::Get()->BuyItem(button->itemData);
		});
		buyButtons.push_back(button);
	}
}

void ShopScene1229::End()
{
	for (auto button : buyButtons) {
		delete button;
	}
	buyButtons.clear();
}

void ShopScene1229::Update()
{
	for (auto button : buyButtons)
		button->Update();

	backButton->Update();
}

void ShopScene1229::Render(HDC hdc)
{
	for (auto button : buyButtons) {
		string str;
		if (button->itemData->category == 0) {
			str = button->itemData->name + " "
				+ to_string(button->itemData->price)
				+ "(소지 :"
				+ to_string(Inventory1229::Get()->GetItemCount(button->itemData->key))
				+ "개)";
			button->SetText(str);
			button->Render(hdc);
		}
		else {
			EquipItemData1229* equipData = (EquipItemData1229*)(button->itemData);
			str = equipData->name + " "
				+ to_string(equipData->price);

			for (auto f : equipData->forces) {
				str += " " + f.first + (f.second >= 0 ? " +" : " ") + to_string(f.second);
			}
			button->SetText(str);
			button->Render(hdc);
		}
	}

	wstring str = L"Money : " + to_wstring(Inventory1229::Get()->GetMoney());
	TextOut(hdc, 100, 200, str.c_str(), str.size());

	backButton->Render(hdc);
}