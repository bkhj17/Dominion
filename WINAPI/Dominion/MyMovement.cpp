#include "framework.h"
#include "MyMovement.h"

MyPointMovement::MyPointMovement(GameObject* owner)
	: owner(owner)
{
}

void MyPointMovement::Update()
{
	if (isMoving) {
		Vector2 v = (targetPos - startPos);
		moved += speed * DELTA;
		moved = Clamp(0, v.Length(), moved);

		if (v.Length() - moved <= tolerance)
			Teleport(targetPos);
		else
			owner->pos = startPos + v.GetNormalized() * Clamp(0, v.Length(), moved);
	}
}

void MyPointMovement::SetTargetPos(Vector2 target, float speed, float tolerance)
{
	this->tolerance = tolerance;

	moved = 0.0f;
	startPos = owner->pos;
	
	if ((target - startPos).Length() < tolerance) {
		Teleport(target);
		return;
	}

	isMoving = true;
	this->speed = speed;
	targetPos = target;
}

void MyPointMovement::SetTargetPosByTime(Vector2 target, float time, float tolerance)
{
	SetTargetPos(target, (target - owner->pos).Length() / time, tolerance);
}

void MyPointMovement::Teleport(Vector2 target)
{
	isMoving = false;
	owner->pos = targetPos = target;
}
