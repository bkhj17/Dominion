#pragma once
class BulletManager : public PoolingManager, public Singleton<BulletManager>
{
private:
	friend class Singleton;
	BulletManager();
	~BulletManager();
public:	
	virtual void CreateObjects(string key, UINT poolSize) override;

	void Fire(const Vector2& pos);
	void Fire(const Vector2& pos, const Vector2& direction);

	virtual void Render(HDC hdc);

	bool Collision(const GameObject* object);
	void CollisionBullet(string key1, string key2);
private:
	HPEN hPen = nullptr;
};

