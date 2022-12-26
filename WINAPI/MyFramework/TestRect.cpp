#include "framework.h"
#include "MyMovement.h"
#include "TestRect.h"

TestRect::TestRect(Vector2 pos, Vector2 size)
	: Rect(pos, size)
{
	movement = new MyMovement(this);
}

TestRect::~TestRect()
{
}

void TestRect::Update()
{
	movement->Update();
}