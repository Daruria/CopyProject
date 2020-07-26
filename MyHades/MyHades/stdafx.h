// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <process.h>

// TODO: reference additional headers your program requires here

#include <vector>
#include <list>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <functional>
#include <fstream>
#include <ctime>

using namespace std;

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include "Define.h"
#include "Enum.h"
#include "Extern.h"
#include "Function.h"
#include "Struct.h"

//	Managers
#include "KeyMgr.h"
#include "DeviceMgr.h"
#include "MathMgr.h"
#include "TextureMgr.h"
#include "ObjectMgr.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "FrameMgr.h"
#include "ScrollMgr.h"
#include "SubjectMgr.h"
#include "FontMgr.h"
#include "ThreadMgr.h"
#include "SoundMgr.h"
#include "AstarMgr.h"

#ifdef _DEBUG
#include <vld.h>
#endif // _DEBUG