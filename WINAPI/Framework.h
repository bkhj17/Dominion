// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
//사용자 정의 상수
#define WIN_START_X 100
#define WIN_START_Y 100

#define WIN_WIDTH	1280
#define WIN_HEIGHT	720

#define CENTER_X	(WIN_WIDTH * 0.5f)
#define CENTER_Y	(WIN_HEIGHT * 0.5f)

#define PI 3.14f

#define RED RGB(255,0,0)
#define GREEN RGB(0,255,0)
#define BLUE RGB(0,0,255)
#define YELLOW RGB(255,255,0)
#define CYAN RGB(0,255,255)
#define MAGENTA RGB(255,0,255)
#define BLACK RGB(0,0,0)
#define WHITE RGB(255,255,255)
// Windows 헤더 파일
#include <windows.h>

// C 런타임 헤더 파일입니다.
/*
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
*/
#include <cmath>
#include <cstring>

// c++ 헤더
#include <iostream>

#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <functional>
using namespace std;

typedef function<void()> Event;
typedef function<void(void*)> ParamEvent;

//FMOD Library
#include "Libraries/inc/fmod.hpp"
#pragma comment(lib, "Libraries/lib/fmod_vc.lib")

//Framework
#include "Framework/Math/Vector2.h"
#include "Framework/Math/GameMath.h"
using namespace GameMath;

#include "Framework/Utilities/Singleton.h"
#include "Framework/Utilities/Timer.h"
#include "Framework/Utilities/KeyBoard.h"
#include "Framework/Utilities/Audio.h"

#include "Framework/Render/Texture.h"
#include "Framework/Render/Animation.h"
#include "Framework/Render/Camera.h"

#include "Framework/Utilities/Utility.h"

using namespace Utility;

#define DELTA			Timer::Get()->GetElapsedTime()

#define KEY_DOWN(k)		KeyBoard::Get()->Down(k)
#define KEY_UP(k)		KeyBoard::Get()->Up(k)
#define KEY_PRESS(k)	KeyBoard::Get()->Press(k)

#define FONT_WIDTH 12
#define FONT_HEIGHT 18

#include "Object/Basic/GameObject.h"
#include "Object/Basic/Rect.h"
#include "Object/Basic/Circle.h"
#include "Object/Basic/Line.h"
#include "Object/Basic/ImageRect.h"
#include "Object/Basic/Effect.h"

#include "Object/UI/ProgressBar.h"
#include "Object/UI/Button.h"

#include "Object/Manager/PoolingManager.h"
#include "Object/Manager/EffectManager.h"
#include "Object/Manager/Observer.h"
#include "Object/Manager/PipeManager.h"
#include "Object/Manager/DataManager.h"

#include "Object/Character/Character.h"

#include "Object/Inventory/Item.h"
#include "Object/Inventory/Panel.h"
#include "Object/Inventory/Store.h"
#include "Object/Inventory/Inventory.h"


//사용자 정의 헤더
#include "WINAPI.h"

//Scenes Header
#include "Scenes/Scene.h"

#include "Dominion/Act.h"
#include "Dominion/MyMovement.h"
#include "Dominion/Card.h"

#include "Dominion/CardDataManager.h"
#include "Dominion/CardManager.h"
#include "Dominion/CardSet.h"

#include "Dominion/ScrollableCardSet.h"
#include "Dominion/SelectWindow.h"

#include "Dominion/GameMaster.h"
#include "Dominion/DominionPlayer.h"

//Manager Header
#include "Manager/SceneManager.h"
#include "Manager/GameManager.h"


extern Vector2 mousePos;
extern HWND hWnd;
extern HINSTANCE hInst;