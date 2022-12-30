#include "framework.h"


ImageRect::ImageRect()
	: texture(nullptr)
{
	camRect = new Rect(pos, size);
}

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

void ImageRect::Render(HDC hdc, int alphaValue, POINT curFrame, bool isTrans)
{
	if (!isActive)
		return;
	if (texture == nullptr)
		return;

	texture->Render(hdc, this, alphaValue, curFrame, isTrans);
}

void ImageRect::Render(HDC hdc)
{
	if (!isActive)
		return;

	if (texture == nullptr)
		return;

	Render(hdc, { 0,0 }, true);
}

void ImageRect::CamRender(HDC hdc, POINT curFrame, bool isTrans)
{
	if (!isActive)
		return;
	if (texture == nullptr)
		return;

	camRect->pos = pos - Camera::Get()->GetPos();
	texture->Render(hdc, camRect, curFrame, isTrans);
}

void ImageRect::CamRender(HDC hdc, int alphaValue, POINT curFrame, bool isTrans)
{

	if (!isActive)
		return;
	if (texture == nullptr)
		return;

	camRect->pos = pos - Camera::Get()->GetPos();
	texture->Render(hdc, camRect, alphaValue, curFrame, isTrans);
}

void ImageRect::SetTexture(Texture* texture)
{

	this->texture = texture;
	
	if (texture == nullptr)
		return;
	
	size = texture->GetSize();
	camRect = new Rect(pos, size);
}
