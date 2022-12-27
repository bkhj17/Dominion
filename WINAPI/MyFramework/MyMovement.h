#pragma once
class MyPointMovement {
public:	
	MyPointMovement(GameObject* owner);
	~MyPointMovement() = default;

	void Update();

	void SetTargetPos(Vector2 vec, float tolerance = 10.0f);
	void SetTargetPos(Vector2 target, float speed = 10.0f, float tolerance = 10.0f);
	void SetTargetPosByTime(Vector2 target, float time, float tolerance = 10.0f);
	
	void Teleport(Vector2 target);

	bool IsMoving() { return isMoving; }

private:
	GameObject* owner;
	
	bool isMoving = false;

	Vector2 targetPos;
	float tolerance = 10.0f;

	float speed = 10.0f;
};