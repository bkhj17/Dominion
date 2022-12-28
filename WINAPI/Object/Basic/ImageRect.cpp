#include "framework.h"


ImageRect::ImageRect(wstring file, UINT frameX, UINT frameY, COLORREF transColor)
{
	texture = Texture::Add(file, frameX, frameY, transColor);
	size = texture->GetSize();
	camRect = new Rect(pos, size);
}

ImageRect::ImageRect(Texture* texture)
	: texture(texture)
{
	size = texture->GetSize();
	camRect = new Rect(pos, size);
}

ImageRect::~ImageRect()
{
	delete camRect;
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
	if (!isActive)
		return;

	Render(hdc, { 0,0 }, true);
}

void ImageRect::CamRender(HDC hdc, POINT curFrame, bool isTrans)
{
	if (!isActive)
		return;

	camRect->pos = pos - Camera::Get()->GetPos();
	texture->Render(hdc, camRect, curFrame, isTrans);
}

void ImageRect::CamRender(HDC hdc, int alpha, POINT curFrame, bool isTrans)
{

	if (!isActive)
		return;

	camRect->pos = pos - Camera::Get()->GetPos();
	texture->Render(hdc, camRect, alpha, curFrame, isTrans);
}
