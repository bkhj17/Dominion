#pragma once
class Circle : public GameObject {
public:
	Circle() = default;
	Circle(Vector2 pos, float radius);

	// GameObject��(��) ���� ��ӵ�
	virtual void Render(HDC hdc) override;

	virtual bool IsPointCollision(const Vector2& point) const override;
	bool IsCircleCollision(const Circle* circle) const;
	bool IsRectCollision(const Rect* rect) const;

	float GetRadius() const { return radius; }

protected:
	float radius = 0.0f;
};