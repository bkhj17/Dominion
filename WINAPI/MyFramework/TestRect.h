#pragma once
class TestRect : public Rect {
public:
	TestRect(Vector2 pos, Vector2 size);
	~TestRect();

	void Update();

	class MyMovement* movement;
private:
};