#pragma once
class Circle;

class Rect : public GameObject
{
public:
	Rect() = default;
	Rect(Vector2 pos, Vector2 size);
	Rect(float left, float top, float right, float bottom);

	virtual void Render(HDC hdc) override;
	void LineRender(HDC hdc);

	virtual bool IsPointCollision(const Vector2& point) const override;
	virtual bool IsRectCollision(const Rect* rect) const override;
	virtual bool IsRectCollision(IN const Rect* rect, OUT Vector2* overlapSize) const;
	virtual bool IsCircleCollision(const Circle* circle) const override;

	float Left() const { return pos.x - size.x * 0.5f; }
	float Right() const { return pos.x + size.x * 0.5f; }
	float Top() const { return pos.y - size.y * 0.5f; }
	float Bottom() const { return pos.y + size.y * 0.5f; }

	Vector2 Half() { return size / 2; }
public:
	Vector2 size;
};

