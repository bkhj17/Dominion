#include "framework.h"

Camera::Camera()
{
	rect = new Rect({}, { WIN_WIDTH, WIN_HEIGHT });
}

Camera::~Camera()
{
	delete rect;
}

void Camera::Update()
{
	if (target != nullptr)
		FollowMode();
	else
		FreeMove();
}

Vector2 Camera::GetPos()
{
	return rect->pos;
}

void Camera::SetPos(Vector2 pos)
{
	rect->pos = pos;
}

void Camera::FreeMove()
{
	if (KEY_PRESS(VK_LBUTTON)) {
		if (KEY_PRESS('A'))
			rect->pos.x -= moveSpeed * DELTA;
		if (KEY_PRESS('D'))
			rect->pos.x += moveSpeed * DELTA;
		if (KEY_PRESS('W'))
			rect->pos.y -= moveSpeed * DELTA;
		if (KEY_PRESS('S'))
			rect->pos.y += moveSpeed * DELTA;
		FixPos(rect->pos);
	}
}

void Camera::FollowMode()
{
	destPos = target->pos - rect->Half() + offset;

	FixPos(destPos);

	rect->pos = Lerp(rect->pos, destPos, followSpeed * DELTA);
}

void Camera::FixPos(Vector2& pos)
{
	if (mapRect == nullptr)
		return;

	if (pos.x < mapRect->Left())
		pos.x = mapRect->Left();

	if (pos.y < mapRect->Top())
		pos.y = mapRect->Top();

	if (pos.x + rect->size.x > mapRect->Right())
		pos.x = mapRect->Right() - rect->size.x;

	if (pos.y + rect->size.y > mapRect->Bottom())
		pos.y = mapRect->Bottom() - rect->size.y;
}
