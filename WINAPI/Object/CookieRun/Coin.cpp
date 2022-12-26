#include "framework.h"
#include "Coin.h"

Coin::Coin()
	: ImageRect(L"Textures/CookieRun/Coin.bmp", 6, 1)
{
	animation = new Animation({ 6, 1 }, 3.0f);
	animation->SetDefault(true);
	animation->Play();
}

Coin::~Coin()
{
	delete animation;
}

void Coin::Update()
{
	if (!isActive) 
		return;

	animation->Update();
}

void Coin::Render(HDC hdc)
{
	if (!isActive)
		return;

	ImageRect::Render(hdc, animation->GetFrame());
}
