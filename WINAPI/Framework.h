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
// c++ 헤더
#include <iostream>

#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;

#include "Basic/Rect.h"

//사용자 정의 헤더
#include "WINAPI.h"
#include "DrawShape.h"

//Scenes Header
#include "Scenes/Scene.h"
#include "Scenes/TutorialScene.h"

//Manager Header
#include "Manager/GameManager.h"