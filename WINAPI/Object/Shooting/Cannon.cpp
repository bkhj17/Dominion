#include "framework.h"
#include "Cannon.h"

Cannon::Cannon()
{
	radius = 50.0f;
	barrel = new Line();
	isActive = true;
}

Cannon::~Cannon()
{
	delete barrel;
}

void Cannon::Update()
{
	if(KEY_PRESS(VK_UP))
		angle += DELTA;

	if (KEY_PRESS(VK_DOWN))
		angle -= DELTA;

	float a = mousePos.x - pos.x;
	float b = mousePos.y - pos.y;
	angle = atan2f(-b, a);

	barrel->end = barrel->pos = pos;	
	barrel->end.x += cosf(angle) * barrelLength;
	barrel->end.y += -sinf(angle) * barrelLength;
}

void Cannon::Render(HDC hdc)
{
	if (!isActive)
		return;
		
	__super::Render(hdc);
	barrel->Render(hdc);
}
