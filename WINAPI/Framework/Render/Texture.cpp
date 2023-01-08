#include "framework.h"

unordered_map<wstring, Texture*> Texture::textures;
bool Texture::isDebugMode = false;

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

void Texture::Remove(wstring file)
{
    if (textures.count(file) == 0)
        return;

    delete textures[file];
    textures.erase(file);
}

void Texture::Delete()
{
    for (pair<wstring, Texture*> texture : textures) {
        delete texture.second;
    }
}

void Texture::Render(HDC hdc, Rect* cardRect, POINT curFrame, bool isTrans)
{
    if (isDebugMode)
        cardRect->LineRender(hdc);

    if (!isTrans) {
        BitBlt(
            hdc,
            (int)cardRect->Left(),
            (int)cardRect->Top(),
            (int)cardRect->size.x,
            (int)cardRect->size.y,
            memDC,
            0, 
            0,
            SRCCOPY
        );
        return;
    }

    GdiTransparentBlt(
            hdc,
            (int)cardRect->Left(),
            (int)cardRect->Top(),
            (int)cardRect->size.x,
            (int)cardRect->size.y,
            memDC,
            cutSize.x * curFrame.x,
            cutSize.y * curFrame.y,
            cutSize.x,
            cutSize.y,
            transColor
    );
}

void Texture::Render(HDC hdc, Rect* cardRect, int alphaValue, POINT curFrame, bool isTrans)
{
    if (isDebugMode)
        cardRect->LineRender(hdc);

    blendFunc.SourceConstantAlpha = alphaValue;

    if (isDebugMode)
        cardRect->LineRender(hdc);
    if (!isTrans) {
        GdiAlphaBlend(
            hdc,
            (int)cardRect->Left(),
            (int)cardRect->Top(),
            (int)cardRect->size.x,
            (int)cardRect->size.y,
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
        (int)cardRect->Left(),
        (int)cardRect->Top(),
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
		(int)cardRect->Left(),
		(int)cardRect->Top(),
		(int)cardRect->size.x,
		(int)cardRect->size.y,
		alphaMemDC,
		0,
		0,
		cutSize.x,
        cutSize.y,
		blendFunc
	);

}

float Texture::GetPixelHeight(const Vector2& pos)
{
    int startY = max((int)pos.y, 0);

    int y = startY;
    for (; y < imageSize.y; y++) {
        COLORREF color = GetPixel(memDC, (int)pos.x, y);
        if (color != transColor) {
            break;
        }
    }

    if (y == startY) {
        for (; y > 0; y--) {
            COLORREF color = GetPixel(memDC, (int)pos.x, y);
            if (color == transColor) {
                return (float)(y+1);
            }
        }
    }

    return (float)y;
}
