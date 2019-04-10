// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 특정 포함 파일이 들어 있는
// 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <time.h>
#include <assert.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>

using namespace std;

#include "LibraryHeader.h"
#include "DefineHeader.h"
#include "EnumStructHeader.h"
#include "ModuleHeader.h"

//콘솔 창 출력
#ifdef _DEBUG
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif
#endif

//extern을 사용해서 변수를 선언하면 전역의 cpp에서 해당 변수를 가져올 수 있다.
extern bool _isDebug;