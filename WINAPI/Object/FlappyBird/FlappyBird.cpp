#include "framework.h"
#include "FlappyBird.h"

FlappyBird::FlappyBird()
	: ImageRect(L"Textures/BlackBird.bmp", 4, 3)
{
	animation = new Animation(texture->GetFrame());
	//animation->SetDefault(true);
	animation->SetPart(0, 7, true);
	animation->Play();

	
	bodyRect = new Rect(Vector2(), Vector2(70, 50));
	bodyOffset = {0, 30};

}

FlappyBird::~FlappyBird()
{
	delete animation;
}

void FlappyBird::Update()
{
	Jump();
	animation->Update();
	bodyRect->pos = pos + bodyOffset;
}

void FlappyBird::Render(HDC hdc)
{
	POINT frame = animation->GetFrame();
	ImageRect::Render(hdc, animation->GetFrame());
	LineRender(hdc);
}

void FlappyBird::Jump()
{
	if (KEY_DOWN(VK_SPACE))
		velocity = JUMP_POWER;

	velocity -= GRAVITY * DELTA;

	pos.y -= velocity * DELTA;
}
