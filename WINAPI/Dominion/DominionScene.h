#pragma once

class DominionScene : public Scene
{
public:
	DominionScene();
	~DominionScene();

	virtual void Start() override;
	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	
	void RenderInfo(HDC hdc);
private:
	class InfoBox* infoBox;
};

