#include "framework.h"

ImageRect::ImageRect(Texture* texture)
	: texture(texture)
{
	size = texture->GetSize();
}

ImageRect::ImageRect(wstring file, UINT frameX, UINT frameY, COLORREF transColor)
{
	texture = Texture::Add(file, frameX, frameY, transColor);
	size = texture->GetSize();
}

void ImageRect::Render(HDC hdc, POINT curFrame, bool isTrans) {
	if (!isActive)
		return;

	texture->Render(hdc, this, curFrame, isTrans);
}

void ImageRect::Render(HDC hdc, int alpha, POINT curFrame, bool isTrans)
{
	if (!isActive)
		return;

	texture->Render(hdc, this, alpha, curFrame, isTrans);
}

void ImageRect::Render(HDC hdc)
{
	Render(hdc, { 0,0 }, true);
}
