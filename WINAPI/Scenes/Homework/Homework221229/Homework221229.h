#pragma once
class Homework221229 : public Scene
{
	//∏ﬁ¿Œ æ¿
public:
	Homework221229();
	~Homework221229();
	// Scene¿ª(∏¶) ≈Î«ÿ ªÛº”µ 
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	Button* shopButton;
	Button* sellSceneButton;
	Button* inventoryButton;
};

