#include "framework.h"
#include "MyMovement.h"

MyPointMovement::MyPointMovement(GameObject* owner)
	: owner(owner)
{
}

void MyPointMovement::Update()
{
	if (isMoving) {
		Vector2 vector = (targetPos - owner->pos);
		if (vector.Length() <= tolerance) {
			Teleport(targetPos);
		}
		else {
			owner->pos += vector.GetNormalized() * speed * DELTA;
		}
	}
}

void MyPointMovement::SetTargetPos(Vector2 target, float speed, float tolerance)
{
	this->tolerance = tolerance;

	Vector2 v = target - owner->pos;
	if (v.Length() < tolerance) {
		Teleport(target);
		return;
	}

	isMoving = true;
	this->speed = speed;
	targetPos = target;
}

void MyPointMovement::SetTargetPosByTime(Vector2 target, float time, float tolerance)
{
	this->tolerance = tolerance;

	Vector2 v = target - owner->pos;
	isMoving = true;
	speed = v.Length() / time;
	targetPos = target;
}

void MyPointMovement::Teleport(Vector2 target)
{
	isMoving = false;
	owner->pos = targetPos = target;
}
