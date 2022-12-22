// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
//사용자 정의 상수
#define WIN_WIDTH	1280
#define WIN_HEIGHT	720

#define CENTER_X	(WIN_WIDTH * 0.5f)
#define CENTER_Y	(WIN_HEIGHT * 0.5f)
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
using namespace std;

//Framework
#include "Framework/Math/Vector2.h"
#include "Framework/Math/GameMath.h"
using namespace GameMath;

#include "Framework/Utilities/Singleton.h"
#include "Framework/Utilities/Timer.h"
#include "Framework/Utilities/KeyBoard.h"

#define DELTA			Timer::Get()->GetElapsedTime()

#define KEY_DOWN(k)		KeyBoard::Get()->Down(k)
#define KEY_UP(k)		KeyBoard::Get()->Up(k)
#define KEY_PRESS(k)	KeyBoard::Get()->Press(k)

#include "Object/Basic/GameObject.h"
#include "Object/Basic/Rect.h"
#include "Object/Basic/Circle.h"
#include "Object/Basic/Line.h"

#include "Object/Spawn/SpawnObject.h"
#include "Object/Spawn/SpawnManager.h"
#include "Object/Shooting/Bullet.h"
#include "Object/Shooting/Plane.h"
#include "Object/Shooting/Enemy.h"

#include "Object/Manager/PoolingManager.h"
#include "Object/Manager/BulletManager.h"
#include "Object/Manager/EnemyManager.h"

#include "Object/Shooting/Cannon.h"

//사용자 정의 헤더
#include "WINAPI.h"

//Scenes Header
#include "Scenes/Scene.h"
#include "Scenes/TutorialScene.h"

//Manager Header
#include "Manager/GameManager.h"

extern Vector2 mousePos;