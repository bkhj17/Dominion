#pragma once
class LandScape {
public:
	LandScape(wstring file);
	~LandScape();

	void Update();
	void Render(HDC hdc);

	void SetSpeed(float speed) { bgSpeed = speed; }
private:

	vector<ImageRect*> bgs;

	float bgSpeed = 1000.0f;
};