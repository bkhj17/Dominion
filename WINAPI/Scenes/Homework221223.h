#pragma once
class Stage221223 {
	const float SIZE_X = 400.0f;
public:
	Stage221223();
	~Stage221223();

	void LoadStage(string filename);

	void Update(Vector2 velocity);
	void Render(HDC hdc, Rect* screen);
	void LandCollision(Rect* rect);
	void DustCollision(Rect* rect);
	bool GoalCollision(Rect* rect);

	vector<ImageRect*> lands;
	vector<ImageRect*> dusts;
	ImageRect* goal;
};

//////////////////////////////////////////////////////////////////

class HomeworkScene221223 : public Scene {
public:
	static int score;

	HomeworkScene221223();
	~HomeworkScene221223();
	// Scene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
private:
	bool isPlaying = true;

	Megaman* megaman;
	Stage221223* stage;
	LandScape* landScape;

	Rect* screen;
};