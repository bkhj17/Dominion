#pragma once

class InventoryScene1229 :  public Scene
{
public:
	InventoryScene1229();
	~InventoryScene1229();
	virtual void Start() override;

	// Scene을(를) 통해 상속됨
	virtual void Update() override;

	virtual void Render(HDC hdc) override;

	virtual void End() override;
private:
	vector<Button*> buttons;
	Button* backButton;
	
	Texture* normal;
	Texture* over;
	Texture* down;

	string strInfo;
};

