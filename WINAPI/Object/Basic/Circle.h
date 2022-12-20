#pragma once
class Circle : public GameObject {
public:
	Circle() = default;
	Circle(Vector2 pos, float radius);

	// GameObject��(��) ���� ��ӵ�
	virtual void Render(HDC hdc) override;

	virtual bool IsCollision(const Vector2& point) const override;
	bool IsCollision(const Circle* circle) const;
	bool IsCollision(Rect* rect) const;

	float GetRadius() const { return radius; }

protected:
	float radius = 0.0f;
};