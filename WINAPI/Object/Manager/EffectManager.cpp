#include "framework.h"
#include "EffectManager.h"

void EffectManager::Play(string key, Vector2 pos)
{
	Effect* effect = dynamic_cast<Effect*>(Pop(key));
	if (effect != nullptr)
		effect->Play(pos);
}

void EffectManager::Add(string key, UINT poolSize, wstring file, int frameX, int frameY, COLORREF transColor, float speed, bool loop)
{
	if (totalObjects.count(key) > 0)
		return;
	Texture* texture = Texture::Add(file, frameX, frameY, transColor);
	Add(key, poolSize, texture, speed, loop);
}

void EffectManager::Add(string key, UINT poolSize, Texture* texture, float speed, bool loop)
{
	if (totalObjects.count(key) > 0)
		return;

	vector<GameObject*> effects(poolSize);

	for (auto& effect : effects) {
		effect = new Effect(texture, speed, loop);
		effect->isActive = false;
	}
	totalObjects[key] = effects;
}
