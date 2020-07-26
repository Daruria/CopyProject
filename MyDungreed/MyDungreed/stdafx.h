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

// TODO: reference additional headers your program requires here

//#include <crtdbg.h>
#include <ctime>
#include <vector>
#include <list>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "Extern.h"
#include "defines.h"
#include "enums.h"
#include "structs.h"

//	Static Class
#include "Math.h"
#include "PlayerUI.h"
#include "Mouse.h"
#include "Inventory.h"
#include "Result.h"

//	Managers
#include "KeyManager.h"
#include "BitmapManager.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "CollisionManager.h"
#include "ScrollManager.h"
#include "TileManager.h"
#include "EffectManager.h"
#include "FontManager.h"
#include "SoundMgr.h"

#ifdef _DEBUG
//	VLD
	#include  "vld.h"
#endif
