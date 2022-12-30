#pragma once
class Line : public GameObject
{
public:
	Line() = default;
	Line(Vector2 start, Vector2 end);


	// GameObject을(를) 통해 상속됨
	virtual void Render(HDC hdc) override;

	virtual bool IsPointCollision(const Vector2& point) const override;

	virtual bool IsRectCollision(const Rect* rect) const override;

	virtual bool IsCircleCollision(const Circle* circle) const override;

	virtual bool IsLineCollision(const Line* line) const override;
	bool IsLineCollision(IN const Line* line, OUT Vector2& crossPoint);
	bool IsCircleCollision(const Circle* circle, OUT Vector2& closestPoint) const;

	bool IsBetweenLine(const Line* line) const;
public:
	Vector2 end;
};

