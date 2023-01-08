#pragma once
class StageManager 
	: public PoolingManager
	, public Singleton<StageManager>
{
private:
	enum TileType {
		NONE, GROUND, COIN, OBSTACLE
	};

	friend class Singleton;
	StageManager();
	~StageManager();

public:
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	float GetGroundHeight(Vector2 pos);
	bool CollisionCoin(Rect* cardRect);
	bool CollisionObstacle(Rect* cardRect);

	void LoadTileData(wstring file);

	float GetSpeed() { return speed; }
	bool IsPlay() { return isPlay; }

	void Play() { isPlay = true; }
	void Stop() { isPlay = false; }
private:
	void CreateTile();

private:
	UINT width = 0, height = 0;
	Vector2 tileSize;

	vector<vector<TileType>> tileData;

	float speed = 500.0f;

	UINT score = 0;

	bool isPlay = true;
};