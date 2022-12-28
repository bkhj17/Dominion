#include "framework.h"

Effect::Effect(wstring file, UINT frameX, UINT frameY, float speed, bool loop, COLORREF trans)
	: ImageRect(file, frameX, frameY, trans)
{
	animation = new Animation({ (LONG)frameX, (LONG)frameY }, speed);
}

Effect::Effect(Texture* texture, float speed, bool loop)
	: ImageRect(texture)
{
	animation = new Animation(texture->GetFrame(), speed);
	animation->SetDefault(loop);
	animation->SetEndEvent(bind(&Effect::Stop, this));
}

Effect::~Effect()
{
	delete animation;
}

void Effect::Update()
{
	__super::Update();
	animation->Update();
}

void Effect::Render(HDC hdc)
{
	__super::CamRender(hdc, animation->GetFrame());
}

void Effect::Play(const Vector2& pos)
{
	this->pos = pos;
	isActive = true;
	animation->Play();
}

void Effect::Stop()
{
	isActive = false;
}
