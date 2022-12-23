#pragma once
class FlappyBird : public ImageRect
{
private:
	const float JUMP_POWER = 500.0f;
	const float GRAVITY = 980.0f;

public:
	FlappyBird();
	~FlappyBird();

	void Update();
	void Render(HDC hdc);

	Rect* BodyRect() { return bodyRect; };
private:
	void Jump();

private:
	float velocity = 0.0f;
	Animation* animation;
	Rect* bodyRect;
	Vector2 bodyOffset;
};

