#include "framework.h"
#include "GameObject.h"

bool GameObject::IsCollision(const GameObject* object) const
{
	if (!isActive || !object->isActive)
		return false;

	const Circle* circle = dynamic_cast<const Circle*>(object);
	if (circle != nullptr)
		return IsCircleCollision(circle);

	const Rect* rect = dynamic_cast<const Rect*>(object);
	if (rect != nullptr)
		return IsRectCollision(rect);

	return false;
}
