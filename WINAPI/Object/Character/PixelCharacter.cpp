#include "framework.h"

PixelCharacter::PixelCharacter(wstring file, int frameX, int frmaeY)
    : ImageRect(file, frameX, frmaeY)
{
}

PixelCharacter::~PixelCharacter()
{
	for (pair<int, Animation*> animation : animations) {
		delete animation.second;
	}
}

void PixelCharacter::Update()
{
	Jump();

	if(animations.count(curType) > 0)
		animations[curType]->Update();
}

void PixelCharacter::Render(HDC hdc)
{
	ImageRect::CamRender(hdc, animations[curType]->GetFrame());
}

bool PixelCharacter::CanMove()
{
	Vector2 nextPos = pos;

	if (velocity.x > 0.0f) {
		nextPos.x += CHECK_DIST;
	}
	else if (velocity.x < 0.0f)
		nextPos.x -= CHECK_DIST;


	if ((nextPos.x < 0.0f && velocity.x < 0.0f)
		|| (nextPos.x > landTexture->GetSize().x && velocity.x > 0.0f)) {
		return false;
	}

	float nextHeight = landTexture->GetPixelHeight(nextPos);
	float bottom = Bottom();
	if (Bottom() - nextHeight > MAX_SLOPE)
		return false;
	return true;
}


void PixelCharacter::Jump()
{
	velocity.y -= GRAVITY * DELTA;
	pos.y -= velocity.y * DELTA;

	Vector2 bottomPos = pos;
	bottomPos.y = Bottom();
	float height = landTexture->GetPixelHeight(bottomPos);

	if (Bottom() > height) {
		if (velocity.y < 0.0f) {
			pos.y = height - Half().y;
			velocity.y = 0.0f;
			jumpCount = 0;
		}
	}
}

void PixelCharacter::SetAction(int type)
{
	if (curType == type)
		return;

	curType = type;
	animations[curType]->Play();
}
