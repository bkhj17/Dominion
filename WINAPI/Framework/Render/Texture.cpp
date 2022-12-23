#include "framework.h"

unordered_map<wstring, Texture*> Texture::textures;

Texture::Texture(wstring file, COLORREF transColor, UINT frameX, UINT frameY)
    : imageFrame{(long)frameX, (long)frameY}, transColor(transColor)
{
    HDC hdc = GetDC(hWnd);
    memDC = CreateCompatibleDC(memDC);
    ReleaseDC(hWnd, hdc);

    hBitmap = (HBITMAP)LoadImage(hInst, file.c_str(), IMAGE_BITMAP,
        0, 0, LR_LOADFROMFILE);

    SelectObject(memDC, hBitmap);

    BITMAP bitmap;
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);
    imageSize = { bitmap.bmWidth, bitmap.bmHeight };

    cutSize = { imageSize.x / imageFrame.x, imageSize.y / imageFrame.y };
}

Texture::~Texture()
{
    DeleteDC(memDC);
    DeleteObject(hBitmap);
}

Texture* Texture::Add(wstring file, COLORREF transColor, UINT frameX, UINT frameY)
{
    if(textures.count(file) > 0)
        return textures[file];

    Texture* texture = new Texture(file, transColor, frameX, frameY);
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
    }
    else {
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

}
