#include "framework.h"
#include "Button.h"

Button::Button(wstring file)
	: ImageRect(file)
{
	normalTexture = texture;
	overTexture = texture;
	downTexture = texture;
}

Button::Button(Texture* texture)
	: ImageRect(texture)
{
	normalTexture = texture;
	overTexture = texture;
	downTexture = texture;
}

Button::~Button()
{
}

void Button::Update()
{
	if (!isActive)
		return;

	if (IsPointCollision(mousePos)) {
		if (KEY_DOWN(VK_LBUTTON)) {
			isClick = true;
		}
		state = (isClick) ? DOWN : OVER;

		if (KEY_UP(VK_LBUTTON) && isClick) {
			if (event) {
				event();
			}
			isClick = false;
		}
	}
	else {
		state = NORMAL;
		if (KEY_UP(VK_LBUTTON)) {
			isClick = false;
		}
	}

	switch (state) {
	case Button::NORMAL:
		texture = normalTexture;
		break;
	case Button::OVER:
		texture = overTexture;
		break;
	case Button::DOWN:
		texture = downTexture;
		break;
	}
}

void Button::Render(HDC hdc)
{
	if (!isActive)
		return;

	ImageRect::Render(hdc);
	int originBkMode = SetBkMode(hdc, 0);
	if (font) {
		oldFont = (HFONT)SelectObject(hdc, font);
	}
	COLORREF oldColor = SetTextColor(hdc, fontColor);

	RECT rect = { Left(), pos.y - (fontSize/2), Right(), pos.y + (fontSize / 2) };
	DrawTextA(hdc, text.c_str(), text.length(), &rect, DT_CENTER);

	SetTextColor(hdc, oldColor);
	if (font) {
		SelectObject(hdc, oldFont);
	}
	SetBkMode(hdc, originBkMode);
}

void Button::SetFont(HFONT font, int fontSize, COLORREF fontColor)
{
	this->font = font;
	this->fontSize = fontSize;
	this->fontColor = fontColor;
}
