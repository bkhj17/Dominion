#pragma once

class Siva : public PixelCharacter
{
private:
	enum ActionType : int {
		IDLE_LEFT, RUN_LEFT,
		IDLE_RIGHT, RUN_RIGHT
	};

	enum State {
		IDLE, PATROL, HIT,
	};

	const float IDLE_RATE = 1.0f;
	const float PATROL_TIME = 2.0f;
	const float BLINK_SPEED = 3000.0f;
	const float HIT_DELAY = 1.0f;
public:
	Siva();
	~Siva();


	void Update();
	virtual void Render(HDC hdc) override;

	void Damage(void *bomb);
private:
	void ActionState();
	void Idle();
	void Patrol();
	void Hit();

	void Move();

	void StartPatrol();

	void CreateAnimations();

private:
	State state = IDLE;

	float hitTime = 0.0f;
	bool isIncreaseAlpha = false;

	float alphaValue = 255.0f;
	float idleTime = 0.0f;

	float patroltime = 0.0f;
};

