#include "framework.h"
#include "GameObject.h"

bool GameObject::IsCollision(const GameObject* object) const
{
	if (!isActive || !object->isActive)
		return false;

	const Circle* circle = dynamic_cast<const Circle*>(object);
	if (circle != nullptr)
		return IsCircleCollision(circle);

	const Rect* cardRect = dynamic_cast<const Rect*>(object);
	if (cardRect != nullptr)
		return IsRectCollision(cardRect);

	const Line* line = dynamic_cast<const Line*>(object);
	if (line != nullptr)
		return IsLineCollision(line);

	return false;
}
