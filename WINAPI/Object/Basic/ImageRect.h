#pragma once
class ImageRect : public Rect {
public:
	ImageRect(Texture* texture);
	ImageRect(wstring file, 
		UINT frameX = 1, 
		UINT frameY = 1, 
		COLORREF transColor = MAGENTA);
	~ImageRect() = default;


	void Render(HDC hdc, POINT curFrame, bool isTrans = true);
	void Render(HDC hdc, int alpha, POINT curFrame, bool isTrans = true);
	virtual void Render(HDC hdc) override;

protected:
	Texture* texture;
};