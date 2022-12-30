#include "framework.h"

Store::Store()
	: Panel(L"Textures/Inventory/Panal.bmp")
{
	CreateItems();
	CreateButtons();

	Observer::Get()->AddParamEvent(
		"StoreItemSelect", 
		bind(&Store::SelectItem, this, placeholders::_1));
}

Store::~Store()
{
}

void Store::Update()
{
	__super::Update();
	purchaseButton->Update();
	purchaseButton->pos = purchaseOffset + pos;

	sellButton->Update();

	sellButton->pos = sellOffset + pos;
}

void Store::Render(HDC hdc)
{
	__super::Render(hdc);
	purchaseButton->Render(hdc);
	sellButton->Render(hdc);
}

void Store::CreateItems()
{
	map<int, ItemData> itemDatas = DataManager::Get()->GetItemDatas();

	int colCount = 5;
	float interval = 10.0f;
	Vector2 startOffset(-255, 40);

	int i = 0;
	for (auto& data : itemDatas) {
		Item* item = new Item(data.second);
		item->size = { 75, 75 };
		item->SetEvent(bind(&Item::ClickStoreItem, item));

		Vector2 coord = Vector2(
			i%colCount,
			i/colCount
		);
		i++;

		item->LocalPos() = startOffset + offset + coord * interval;
		item->LocalPos().x += coord.x * item->size.x;
		item->LocalPos().y += coord.y * item->size.y;
		items.push_back(item);
	}
}

void Store::CreateButtons()
{
	Texture* normal = Texture::Add(L"Textures/UI/NormalButton.bmp");
	Texture* over = Texture::Add(L"Textures/UI/OverButton.bmp");
	Texture* down = Texture::Add(L"Textures/UI/DownButton.bmp");

	purchaseButton = new Button(normal);
	purchaseButton->SetOverTexture(over);
	purchaseButton->SetDownTexture(down);
	purchaseButton->SetText("구매");
	purchaseButton->SetEvent(bind(&Store::OnClickPurchaseButton, this));

	sellButton = new Button(normal);
	sellButton->SetOverTexture(over);
	sellButton->SetDownTexture(down);
	sellButton->SetText("판매");
	sellButton->SetEvent(bind(&Store::OnClickSellButton, this));
}

void Store::OnClickPurchaseButton()
{
	Observer::Get()->ExcuteParamEvents("PurchaseItem", selectItem);
}

void Store::OnClickSellButton()
{
	Observer::Get()->ExcuteEvents("SellItem");
}
