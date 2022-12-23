#pragma once
class LandScape {
public:
	LandScape();
	~LandScape();

	void Update();
	void Render(HDC hdc);

	void SetBgSpeed(float speed) { bgSpeed = speed; }
private:

	vector<ImageRect*> bgs;

	float bgSpeed = 1000.0f;
};