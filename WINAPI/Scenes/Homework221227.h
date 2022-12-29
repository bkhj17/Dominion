#pragma once
#include "Scene.h"


////////////////////////////////////////////////////

enum class Rank1227 {
	PERFECT, GREAT, GOOD, MISS
};

class Drum1227 : public Circle {
private:
	const float RADIUS = 50.0f;
public:
	Drum1227();
	~Drum1227();

	void Update();
	void Render(HDC hdc);
private:
};

class NodeManager : public Singleton<NodeManager>
{
	const float RADIUS = 70.0f;
	const UINT POOL_SIZE = 50;
private:
	friend class Singleton;
	NodeManager();
	~NodeManager();

	void CreateObjects(string key, UINT poolSize);

public:
	void Update();
	void Render(HDC hdc);

	void Collision(Circle* circle);
	void SetMessageFunc(function<void(Rank1227)> func) { scoringFunc = func; }

private:
	void Spawn();

	float spawnRate = 1.0f;
	float spawnTime = 0.0f;

	float moveSpeed = 300.0f;
	float radius = 50.0f;

	function<void(Rank1227)> scoringFunc;
private:
	int head = 0;
	int tail = 0;
	vector<Circle*> nodes;
};

class EffectRect : public ImageRect {
public:
	EffectRect();
	~EffectRect();

	void Play(Rank1227 rank);

	void Update();
	void Render(HDC hdc);
private:
	void CreateAnimations();
	void EndEffect() { isActive = false; }

	Rank1227 curAnim = Rank1227::MISS;
	map<Rank1227, Animation*> animations;
};

class Homework221227 : public Scene
{
public:

	Homework221227();
	~Homework221227();

	// Scene을(를) 통해 상속됨
	virtual void Init() override;

	virtual void Update() override;

	virtual void Render(HDC hdc) override;

	void Scoring(Rank1227 rank);
	void SetMessage(wstring msg) { message = msg; }

private:
	Drum1227* drum;
	EffectRect* effect;
	wstring message = L"";

	Rect* lane;
	HBRUSH hBrush;

	ImageRect* bg;
};
