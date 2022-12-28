#include "framework.h"

ProgressBar::ProgressBar(Texture* front, Texture* back)
	: frontTexture(front), backTexture(back)
{
	size = frontTexture->GetSize();
}

ProgressBar::~ProgressBar()
{
}

void ProgressBar::Render(HDC hdc)
{
	if (backTexture != nullptr)
		backTexture->Render(hdc, this);

    GdiTransparentBlt(
        hdc,
        (int)Left(), (int)Top(),
        (int)(size.x * value),
        (int)size.y,
        frontTexture->GetMemDC(),
        0, 0,
        (int)(frontTexture->GetSize().x * value),
        (int)frontTexture->GetSize().y,
        MAGENTA
    );
}
