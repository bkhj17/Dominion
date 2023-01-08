#include "framework.h"
#include "Dominion/MyMovement.h"
#include "TestRect.h"
#include "TestScene.h"

TestScene::TestScene()
{
	cardRect = new TestRect({ CENTER_X, CENTER_Y }, {100.0f, 100.0f});
}

TestScene::~TestScene()
{
	delete cardRect;
}

void TestScene::Start()
{
	cardRect->pos = { CENTER_X, CENTER_Y };
}

void TestScene::Update()
{
	
	if (requestControl) {
		if (KEY_DOWN(VK_LBUTTON)) {
			cardRect->movement->SetTargetPosByTime(mousePos, 2.0f);
			requestControl = false;
		}
	}


	cardRect->Update();
	if (!cardRect->movement->IsMoving()) {
		requestControl = true;
	}

}

void TestScene::Render(HDC hdc)
{
	cardRect->Render(hdc);
}
