#pragma once
class MyPointMovement {
public:	
	MyPointMovement(GameObject* owner);
	~MyPointMovement() = default;

	void Update();

	void SetTargetPos(Vector2 target, float speed = 100.0f, float tolerance = 1.0f);
	void SetTargetPosByTime(Vector2 target, float time = 1.0f, float tolerance = 1.0f);
	
	void Teleport(Vector2 target);

	bool IsMoving() { return isMoving; }

private:
	GameObject* owner;
	
	bool isMoving = false;

	Vector2 startPos;
	Vector2 targetPos;
	float tolerance = 10.0f;	//오차범위

	float speed = 0.0f;
	
	float moved = 0.0f;
	float length = 0.0f;
};