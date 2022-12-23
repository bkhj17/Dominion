#pragma once
class ImageRect : public Rect {
public:
	ImageRect(Texture* texture);
	ImageRect(wstring file, 
		UINT frameX = 1, 
		UINT frameY = 1, 
		COLORREF transColor = MAGENTA);
	~ImageRect() = default;

	void Render(HDC hdc, bool isTrans = false, POINT curFrame = { 0,0 });

protected:
	Texture* texture;
};