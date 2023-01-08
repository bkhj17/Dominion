#pragma once
class Line;
class Circle : public GameObject {
public:
	Circle() = default;
	Circle(Vector2 pos, float radius);

	// GameObject을(를) 통해 상속됨
	virtual void Render(HDC hdc) override;

	virtual bool IsPointCollision(const Vector2& point) const override;
	bool IsCircleCollision(const Circle* circle) const;
	bool IsRectCollision(const Rect* cardRect) const;
	virtual bool IsLineCollision(const Line* line) const override;

	float GetRadius() const { return radius; }

protected:
	float radius = 0.0f;

};