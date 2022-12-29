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

#include "Object/Basic/GameObject.h"
#include "Object/Basic/Rect.h"
#include "Object/Basic/Circle.h"
#include "Object/Basic/Line.h"
#include "Object/Basic/ImageRect.h"
#include "Object/Basic/Effect.h"

#include "Object/UI/ProgressBar.h"
#include "Object/UI/Button.h"

#include "Object/Spawn/SpawnObject.h"
#include "Object/Spawn/SpawnManager.h"

#include "Object/Manager/PoolingManager.h"
#include "Object/Manager/EffectManager.h"
#include "Object/Manager/Observer.h"
#include "Object/Manager/PipeManager.h"

#include "Object/Shooting/Bullet.h"
#include "Object/Shooting/BulletManager.h"
#include "Object/Shooting/Plane.h"
#include "Object/Shooting/Enemy.h"
#include "Object/Shooting/EnemyManager.h"
#include "Object/Shooting/Cannon.h"


#include "Object/Character/Character.h"
#include "Object/Character/PixelCharacter.h"
#include "Object/Character/Megaman.h"
#include "Object/Character/Siva.h"
#include "Object/Character/BattleCharacter.h"
#include "Object/Character/Fighter.h"
#include "Object/Character/Monster.h"



#include "Object/BrickOut/BrickManager.h"
#include "Object/BrickOut/ControlBar.h"
#include "Object/BrickOut/Ball.h"
#include "Object/BrickOut/Brick.h"

#include "Object/FlappyBird/LandScape.h"
#include "Object/FlappyBird/FlappyBird.h"

#include "Object/CookieRun/Coin.h"
#include "Object/CookieRun/Cookie.h"
#include "Object/CookieRun/StageManager.h"

#include "Object/Dig/Bomb.h"
#include "Object/Dig/BoomManager.h"


#include "Scenes/Homework/Homework221229/DataManager1229.h"
#include "Scenes/Homework/Homework221229/Inventory1229.h"

//사용자 정의 헤더
#include "WINAPI.h"

//Scenes Header
#include "Scenes/Scene.h"
#include "Scenes/TutorialScene.h"

//Manager Header
#include "Manager/SceneManager.h"
#include "Manager/GameManager.h"


extern Vector2 mousePos;
extern HWND hWnd;
extern HINSTANCE hInst;