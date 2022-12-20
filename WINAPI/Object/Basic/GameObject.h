#pragma once
class GameObject
{
public:
	GameObject() = default;
	GameObject(Vector2 pos) : pos(pos) {};
	~GameObject() = default;

	virtual void Render(HDC hdc) = 0;

	virtual bool IsCollision(const Vector2& point) const = 0;

	const bool& Active() const { return isActive; }
	Vector2& Pos() { return pos; }
protected:
	bool isActive = true;
	Vector2 pos;
};

