#include "Framework.h"

BOOL DrawShape::Draw(HDC hdc, int x, int y, int width, int height, HPEN hPen, BOOL (func)(HDC hdc, int x, int y, int width, int height))
{
    if (hPen != nullptr) {
        auto originObject = SelectObject(hdc, hPen);
        bool result = func(hdc, x, y, x + width, y + height);
        SelectObject(hdc, originObject);
        return result;
    }
    else
        return func(hdc, x, y, x+width, y+height);
}

BOOL DrawShape::DrawRect(HDC hdc, int x, int y, int width, int height, HPEN hPen)
{
    return Draw(hdc, x, y, width, height, hPen, Rectangle);
}

BOOL DrawShape::DrawEllipse(HDC hdc, int x, int y, int width, int height, HPEN hPen)
{
    return Draw(hdc, x, y, width, height, hPen, Ellipse);
}
