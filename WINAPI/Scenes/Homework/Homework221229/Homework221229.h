#pragma once
class Homework221229 : public Scene
{
	//���� ��
public:
	Homework221229();
	~Homework221229();
	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	Button* shopButton;
	Button* sellSceneButton;
	Button* inventoryButton;
};

