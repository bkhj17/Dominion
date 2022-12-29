#pragma once
class BuyButton : public Button {
public:
	BuyButton(Texture* texture) : Button(texture) {}

	struct ItemData1229* itemData = nullptr;
private:
};


class ShopScene1229 : public Scene
{
	//ªÛ¡° æ¿
	enum class ShopState {
		MENU_SELECT,
		BUY,
		SELL,
		EXIT
	};

public:
	ShopScene1229();
	~ShopScene1229();

	virtual void Start() override; 
	virtual void End() override;

	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	//
	vector<BuyButton*> buyButtons;

	Button* backButton;

	Texture* normal;
	Texture* over;
	Texture* down;
};