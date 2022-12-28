#pragma once
class EffectManager : public PoolingManager, public Singleton<EffectManager> {
private:
	friend class Singleton;
	EffectManager() = default;
	~EffectManager() = default;
public:
	void Play(string key, Vector2 pos);
	void Add(string key, UINT poolSize, wstring file, int frameX, int frameY, COLORREF transColor = MAGENTA, float speed = 3.0f, bool loop = false);
	void Add(string key, UINT poolSize, Texture* texture, float speed = 1.0f, bool loop = false);
};