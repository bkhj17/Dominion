#pragma once

class Rect;
class Circle;

class GameObject
{
public:
	GameObject() = default;
	GameObject(Vector2 pos) : pos(pos) {};
	~GameObject() = default;

	virtual void Update() {}
	virtual void Render(HDC hdc) = 0;


	virtual bool IsCollision(const GameObject* object) const;
	virtual bool IsPointCollision(const Vector2& point) const = 0;
	virtual bool IsRectCollision(const Rect* rect) const = 0;	
	virtual bool IsRectCollision(IN const Rect* rect, OUT Vector2* overlapRect) { return false; }
	virtual bool IsCircleCollision(const Circle* circle) const = 0;

public:
	string tag;
	bool isActive = true;
	Vector2 pos;
};

