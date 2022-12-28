#pragma once
class Monster : public BattleCharacter {
protected:
	enum ActionType : int {
		IDLE, RUN, ATTACK, HIT, DEATH
	};

	enum class State {
		PATROL, TRACE, ATTACK, HIT, DEATH
	};

	const float TRACE_RANGE = 300.0f;
	const float ATTACK_RANGE = 200.0f;
	const float PATROL_RANGE = 100.0f;
public:
	Monster();
	~Monster();

	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void Damage();

	void SetTarget(GameObject* target) { this->target = target; }
private:
	virtual void Move() override;

	void SetState();
	void ActionState();

	void Patrol();
	void Trace();
	void Attack();
	void Hit();
	void Death();

	void SetAnimation();
	void CreateAnimations();

private:
	Vector2 destPos;
	float speed = 100.0f;

	State state;

	GameObject* target = nullptr;

	GameObject* traceRange;
	GameObject* attackRange;

	float curHp = 200.0f, maxHp = 200.0f;
	ProgressBar* hpBar;
};