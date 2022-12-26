#pragma once
class Cookie : public Rect
{
private:
	enum class ActionType {
		IDLE, RUN, JUMP, DOUBLE_JUMP, JUMP_DOWN, CRASH
	};

	const float JUMP_POWER = 700.0f;
	const float GRAVITY = 1500.0f;
	const float BOTTOM_CHECK = 10.0f;

	const float GHOST_TIME = 5.0f;
	const float ALPHA_SPEED = 100.0f;
public:
	Cookie();
	~Cookie();

	virtual void Update() override;
	virtual void Render(HDC hdc) override;
private:
	void Jump();
	void Crash();
	void EndCrash();

	void SetAction(ActionType type);
	void SetAnimation();
	void CreateAnimations();
private:
	Texture* texture;
	ImageRect* imageRect;
	Vector2 imageOffset;

	map<ActionType, Animation*> animations;
	ActionType curType = ActionType::JUMP;

	float velocity = 0.0f;
	int jumpCount = 0;

	bool isGhost = false;
	float ghostTime = 0.0f;

	bool isIncreaseAlpha = false;
	float alphaValue = 255.0f;
};

