#include "framework.h"
#include "Character.h"


Character::Character(wstring file, int frameX, int frameY, COLORREF trans)
{
	texture = Texture::Add(file, frameX, frameY, trans);
	imageRect = new ImageRect(texture);

}

Character::~Character()
{
	delete imageRect;
	for (auto& animation : animations)
		delete animation.second;
}

void Character::Update()
{
	imageRect->pos = pos + imageOffset;

	animations[curType]->Update();
}

void Character::Render(HDC hdc, int alphaValue)
{
	imageRect->LineRender(hdc);
	if (animations.count(curType) == 0)
	{
		imageRect->Render(hdc);
		return;
	}

	if (alphaValue < 255) {
		imageRect->Render(hdc, (int)alphaValue, animations[curType]->GetFrame());
	}
	else {
		imageRect->Render(hdc, animations[curType]->GetFrame());
	}

	LineRender(hdc);
}

void Character::SetAction(int type)
{
	if (curType == type)
		return;

	curType = type;
	animations[curType]->Play();
}
