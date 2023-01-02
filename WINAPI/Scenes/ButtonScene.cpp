#include "framework.h"
#include "ButtonScene.h"

ButtonScene::ButtonScene()
{
	hFont = CreateFont(40, 0, 0, 0, 1, false, false, false, HANGUL_CHARSET,
		100,0,0,0,L"배달의 민족 한나체");
	//HANGEUL_CHARSET도 같은거
	LoadFile();

	Texture* normal = Texture::Add(L"Textures/UI/NormalButton.bmp");
	Texture* over = Texture::Add(L"Textures/UI/OverButton.bmp");
	Texture* down = Texture::Add(L"Textures/UI/DownButton.bmp");

	battleButton = new Button(normal);
	battleButton->SetOverTexture(over);
	battleButton->SetDownTexture(down);
	battleButton->pos = Vector2(CENTER_X, CENTER_Y - 100.0f);
	battleButton->SetText("배틀씬");
	battleButton->SetEvent(bind(&ButtonScene::StartBattleScene, this));

	digButton = new Button(normal);
	digButton->SetOverTexture(over);
	digButton->SetDownTexture(down);
	digButton->pos = Vector2(CENTER_X, CENTER_Y + 100.0f);
	digButton->SetText("땅파기 게임");
	digButton->SetEvent(bind(&ButtonScene::StartDigScene, this));

}

ButtonScene::~ButtonScene()
{
	DeleteObject(hFont);
}

void ButtonScene::Update()
{
	battleButton->Update();
	digButton->Update();
}

void ButtonScene::Render(HDC hdc)
{
	auto defaultFont = SelectObject(hdc, hFont);
	wstring str = L"StartScene";
	TextOut(hdc, CENTER_X, CENTER_Y, str.c_str(), str.size());
	SelectObject(hdc, defaultFont);

	battleButton->Render(hdc);
	digButton->Render(hdc);
}

void ButtonScene::StartBattleScene()
{
	SceneManager::Get()->ChangeScene("Battle");
}

void ButtonScene::StartDigScene()
{
	SceneManager::Get()->ChangeScene("DigGame");
}

void ButtonScene::LoadFile()
{
	HANDLE handle;
	WIN32_FIND_DATA findData;
	handle = FindFirstFile(L"Textures/UI/Item/*.bmp", &findData);

	bool result = true;
	vector<wstring> fileList;
	while (result) {
		fileList.push_back(findData.cFileName);
		result = FindNextFile(handle, &findData);
	}
}
