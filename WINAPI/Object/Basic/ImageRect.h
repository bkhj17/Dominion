#pragma once
class ImageRect : public Rect {
public:
	ImageRect(wstring file, 
		UINT frameX = 1, 
		UINT frameY = 1, 
		COLORREF transColor = MAGENTA);
	ImageRect(Texture* texture);
	~ImageRect();


	void Render(HDC hdc, POINT curFrame, bool isTrans = true);
	void Render(HDC hdc, int alphaValue, POINT curFrame, bool isTrans = true);
	virtual void Render(HDC hdc) override;

	void CamRender(HDC hdc, POINT curFrame = {0,0}, bool isTrans = true);
	void CamRender(HDC hdc, int alphaValue, POINT curFrame = {0,0}, bool isTrans = true);

	Texture* GetTexture() { return texture; }
protected:
	Texture* texture;
	Rect* camRect;
};