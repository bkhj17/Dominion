#include "framework.h"

Camera::Camera()
{
	cardRect = new Rect({}, { WIN_WIDTH, WIN_HEIGHT });
}

Camera::~Camera()
{
	delete cardRect;
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
	return cardRect->pos;
}

void Camera::SetPos(Vector2 pos)
{
	cardRect->pos = pos;
}

void Camera::FreeMove()
{
	if (KEY_PRESS(VK_LBUTTON)) {
		if (KEY_PRESS('A'))
			cardRect->pos.x -= moveSpeed * DELTA;
		if (KEY_PRESS('D'))
			cardRect->pos.x += moveSpeed * DELTA;
		if (KEY_PRESS('W'))
			cardRect->pos.y -= moveSpeed * DELTA;
		if (KEY_PRESS('S'))
			cardRect->pos.y += moveSpeed * DELTA;
		FixPos(cardRect->pos);
	}
}

void Camera::FollowMode()
{
	destPos = target->pos - cardRect->Half() + offset;

	FixPos(destPos);

	cardRect->pos = Lerp(cardRect->pos, destPos, followSpeed * DELTA);
}

void Camera::FixPos(Vector2& pos)
{
	if (mapRect == nullptr)
		return;

	if (pos.x < mapRect->Left())
		pos.x = mapRect->Left();

	if (pos.y < mapRect->Top())
		pos.y = mapRect->Top();

	if (pos.x + cardRect->size.x > mapRect->Right())
		pos.x = mapRect->Right() - cardRect->size.x;

	if (pos.y + cardRect->size.y > mapRect->Bottom())
		pos.y = mapRect->Bottom() - cardRect->size.y;
}
