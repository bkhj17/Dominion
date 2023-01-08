#pragma once
class Ball : public Circle {
private:
	const float BALL_SIZE = 10.0f;
public:
	Ball();
	~Ball();

	void Update();
	void Render(HDC hdc);

	void SetBarRect(Rect* cardRect) { barRect = cardRect; }

private:
	void Move();
	void AimControl();

	void CollisionBorder();	//¿Ü°û Æ¨±â±â
	void CollisionBar();
	void CollisionBrick();
private:
	float speed = 400.0f;
	Vector2 direction;

	HBRUSH hBrush;

	Rect* barRect;

	bool isPlay = false;

	Line* aimingDirection;
	float aimLength = 100.0f;
	float aimingAngle = PI * 0.5f;
};