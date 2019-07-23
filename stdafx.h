// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 특정 포함 파일이 들어 있는
// 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <windowsx.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// 여기서 프로그램에 필요한 추가 헤더를 참조합니다.
#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include "Astar_algo.h"
#include "Resource.h"

using std::cout;
using std::endl;
using std::vector;
using std::queue;
using std::priority_queue;
using std::pair;

static const int START_X = 10;
static const int START_Y = 10;
static const int GRID_START_X = 60;
static const int GRID_START_Y = 60;
static const int GRID_WIDTH = 700;
static const int GRID_HEIGHT = 700;
static const int RECT_SIZE = 60;