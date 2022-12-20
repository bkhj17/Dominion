#pragma once
class Circle;

class Rect : public GameObject
{
public:
	Rect() = default;
	Rect(Vector2 pos, Vector2 size);
	Rect(float left, float top, float right, float bottom);

	void Render(HDC hdc) override;

	virtual bool IsCollision(const Vector2& point) const override;
	bool IsCollision(const Circle* point) const;
	bool IsCollision(const Rect* other) const;

	float Left() const { return pos.x - size.x * 0.5f; }
	float Right() const { return pos.x + size.x * 0.5f; }
	float Top() const { return pos.y - size.y * 0.5f; }
	float Bottom() const { return pos.y + size.y * 0.5f; }


	const Vector2& GetSize() { return size; }
protected:

	Vector2 size;

};

