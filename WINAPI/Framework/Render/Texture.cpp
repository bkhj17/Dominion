#include "framework.h"

unordered_map<wstring, Texture*> Texture::textures;

Texture::Texture(wstring file, UINT frameX, UINT frameY, COLORREF transColor)
    : imageFrame{(long)frameX, (long)frameY}, transColor(transColor)
{
    HDC hdc = GetDC(hWnd);
    memDC = CreateCompatibleDC(hdc);
    alphaMemDC = CreateCompatibleDC(hdc);
    hBitmap = (HBITMAP)LoadImage(hInst, file.c_str(), IMAGE_BITMAP,
        0, 0, LR_LOADFROMFILE);

    SelectObject(memDC, hBitmap);

    BITMAP bitmap;
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);
    imageSize = { bitmap.bmWidth, bitmap.bmHeight };

    cutSize = { imageSize.x / imageFrame.x, imageSize.y / imageFrame.y };

    hAlphaBitmap = CreateCompatibleBitmap(hdc, imageSize.x, imageSize.y);
    SelectObject(alphaMemDC, hAlphaBitmap);
    
    ReleaseDC(hWnd, hdc);
}

Texture::~Texture()
{
    DeleteDC(memDC);
    DeleteObject(hBitmap);
}

Texture* Texture::Add(wstring file, UINT frameX, UINT frameY, COLORREF transColor)
{
    if(textures.count(file) > 0)
        return textures[file];

    Texture* texture = new Texture(file, frameX, frameY, transColor);
    textures[file] = texture;
    return texture;
}

void Texture::Delete()
{
    for (pair<wstring, Texture*> texture : textures) {
        delete texture.second;
    }
}

void Texture::Render(HDC hdc, Rect* rect, POINT curFrame, bool isTrans)
{

    if (!isTrans) {
        BitBlt(
            hdc,
            (int)rect->Left(),
            (int)rect->Top(),
            (int)rect->size.x,
            (int)rect->size.y,
            memDC,
            0, 
            0,
            SRCCOPY
        );
        return;
    }

    GdiTransparentBlt(
            hdc,
            (int)rect->Left(),
            (int)rect->Top(),
            (int)rect->size.x,
            (int)rect->size.y,
            memDC,
            cutSize.x * curFrame.x,
            cutSize.y * curFrame.y,
            cutSize.x,
            cutSize.y,
            transColor
        );
}

void Texture::Render(HDC hdc, Rect* rect, int alpha, POINT curFrame, bool isTrans)
{
    blendFunc.SourceConstantAlpha = alpha;

    if (!isTrans) {
        GdiAlphaBlend(
            hdc,
            (int)rect->Left(),
            (int)rect->Top(),
            (int)rect->size.x,
            (int)rect->size.y,
            memDC,
            cutSize.x * curFrame.x,
            cutSize.y * curFrame.y,
            cutSize.x,
            cutSize.y,
            blendFunc
        );
        return;
    }

    BitBlt(alphaMemDC,
        0,
        0,
        cutSize.x,
        cutSize.y,
        hdc,
        (int)rect->Left(),
        (int)rect->Top(),
        SRCCOPY
    );

	GdiTransparentBlt(
		alphaMemDC,
		0,
		0,
		cutSize.x,
        cutSize.y,
		memDC,
		cutSize.x * curFrame.x,
		cutSize.y * curFrame.y,
		cutSize.x,
		cutSize.y,
		transColor
	);
	GdiAlphaBlend(
		hdc,
		(int)rect->Left(),
		(int)rect->Top(),
		(int)rect->size.x,
		(int)rect->size.y,
		alphaMemDC,
		0,
		0,
		cutSize.x,
        cutSize.y,
		blendFunc
	);

}
