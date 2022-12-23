#pragma once
class Rect;

class Texture 
{
private:	//
	Texture(wstring file, COLORREF transColor, UINT frameX, UINT frameY);
	~Texture();

public:
	static Texture* Add(
		wstring file, 
		COLORREF transColor = MAGENTA,
		UINT frameX = 1,
		UINT frameY = 1
	);
	static void Delete();

	void Render(HDC hdc, Rect* rect, POINT curFrame = {0,0}, bool isTrans = true);

	Vector2 GetSize() { return Vector2(cutSize); }
	POINT GetFrame() { return imageFrame; }
private:
	HDC memDC;	//참조
	HBITMAP hBitmap;

	COLORREF transColor = MAGENTA;	//크로마키

	POINT imageFrame;
	POINT imageSize;
	POINT cutSize;

	static unordered_map<wstring, Texture*> textures;
};