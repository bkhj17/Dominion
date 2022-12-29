#include "framework.h"

BagItem1229::BagItem1229(ItemData1229* data)
	: data(data)
{
}

BagEquip1229::BagEquip1229(ItemData1229* data)
	: BagItem1229(data)
{
}

BagUsable1229::BagUsable1229(ItemData1229* data, int count)
	: BagItem1229(data), count(count)
{
}



/// <summary>
/// //////////////////////////////////////////////////////////
/// </summary>


bool Inventory1229::BuyItem(ItemData1229* data)
{
	if (data == nullptr)
		return false;

	if (!UseMoney(data->price)) {
		return false;
	}

	AddItem(data);
	return true;
}

bool Inventory1229::UseMoney(int pay)
{
	if (money < pay)
		return false;

	money -= pay;
	return true;
}

int Inventory1229::GetItemCount(int itemKey)
{
	int i = HaveItem(itemKey);
	if (i == items.size())
		return 0;

	return ((BagUsable1229*)items[i])->count;
}

void Inventory1229::GainMoney(int gain)
{
	money += gain;
}

void Inventory1229::SellItem(int index)
{
	if (index >= items.size())
		return;

	GainMoney(items[index]->data->price);
	BagUsable1229* usable = (BagUsable1229*)items[index];
	usable->count--;
	if (usable->count == 0) {
		items.erase(items.begin()+index);
	}
}


void Inventory1229::AddItem(ItemData1229* data)
{
	int itemIndex = HaveItem(data->key);
	if (itemIndex >= items.size()) {
		BagUsable1229* usable = new BagUsable1229(data, 1);
		items.push_back(usable);
	}
	else {
		BagUsable1229* usable = (BagUsable1229*)(items[itemIndex]);
		if (usable)
			usable->count++;
	}
}

int Inventory1229::HaveItem(int itemKey)
{
	int i = 0;
	for (; i < items.size(); i++) {
		if (items[i]->data->key == itemKey)
			break;
	}
	return i;
}
