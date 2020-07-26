#ifndef Engine_Defines_h__
#define Engine_Defines_h__

#include <d3d9.h>
#include <d3dx9.h>


#include <io.h>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <functional>
#include <ctime>
#include <queue>
#include <stack>
#include <set>
#include <deque>
#include <intrin.h>
#include <process.h>

#pragma warning(disable : 4251)

#define	DIRECTINPUT_VERSION		0x0800
#include <dinput.h>

#include "Engine_Typedef.h"
#include "Engine_Macro.h"

#include "Engine_Function.h"
#include "Engine_Enum.h"
#include "Engine_Struct.h"

using namespace std;

#ifndef _TOOL

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

#endif //_TOOL






#endif // engine_defines_h__