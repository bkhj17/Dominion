#pragma once
class Bullet : public Circle {
private:
	const float SIZE = 20;

public:
	Bullet();
	~Bullet();

	void Update();
	virtual void Render(HDC hdc) override;

	void Fire(const Vector2& pos, Vector2 direction = Vector2(0.0f, -1.0f));
private:
	float speed = 500.0f;
	Vector2 direction;

};