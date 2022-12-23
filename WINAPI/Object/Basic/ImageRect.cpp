#include "framework.h"

ImageRect::ImageRect(Texture* texture)
	: texture(texture)
{
	size = texture->GetSize();
}

ImageRect::ImageRect(wstring file, UINT frameX, UINT frameY, COLORREF transColor)
{
	texture = Texture::Add(file, transColor, frameX, frameY);
	size = texture->GetSize();
}

void ImageRect::Render(HDC hdc, bool isTrans, POINT curFrame) {
	if (!isActive)
		return;

	texture->Render(hdc, this, curFrame, isTrans);
}
