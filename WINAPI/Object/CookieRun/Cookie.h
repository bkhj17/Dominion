#pragma once
class Cookie : public Character
{
private:
	enum class ActionType {
		IDLE, RUN, JUMP, DOUBLE_JUMP, JUMP_DOWN, CRASH
	};
public:
	Cookie();
	~Cookie();

	virtual void Update() override;
	virtual void Render(HDC hdc) override;
private:
	void Jump();
	void Crash();
	void EndCrash();

	void SetAnimation();
	void CreateAnimations();
private:
	ActionType curType = ActionType::JUMP;

	int jumpCount = 0;


	bool isIncreaseAlpha = false;
	float alphaValue = 255.0f;
};

