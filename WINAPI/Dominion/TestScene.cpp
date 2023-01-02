#include "framework.h"
#include "Dominion/MyMovement.h"
#include "TestRect.h"
#include "TestScene.h"

TestScene::TestScene()
{
	rect = new TestRect({ CENTER_X, CENTER_Y }, {100.0f, 100.0f});
}

TestScene::~TestScene()
{
	delete rect;
}

void TestScene::Start()
{
	rect->pos = { CENTER_X, CENTER_Y };
}

void TestScene::Update()
{
	
	if (requestControl) {
		if (KEY_DOWN(VK_LBUTTON)) {
			rect->movement->SetTargetPosByTime(mousePos, 2.0f);
			requestControl = false;
		}
	}


	rect->Update();
	if (!rect->movement->IsMoving()) {
		requestControl = true;
	}

}

void TestScene::Render(HDC hdc)
{
	rect->Render(hdc);
}
