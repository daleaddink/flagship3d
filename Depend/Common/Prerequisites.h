#ifndef _PREREQUISITES_H_
#define _PREREQUISITES_H_

// warning C4251 needs to have dll-interface to be used by clients of class
#pragma warning( disable : 4251 )
// warning C4819: The file contains a character that cannot be represented in the current code page (936)
#pragma warning( disable : 4819 )
// warning C4800: 'BOOL' : forcing value to bool 'true' or 'false'
#pragma warning( disable : 4800 )

#define _DLL_Export __declspec(dllexport)

#include <iostream>
#include <fstream>
#include <io.h>

#include <vector>
#include <list>
#include <stack>
#include <map>
#include <string>
#include <cmath>

#include "../Foundation/Wm4Foundation.h"
#include "../TinyXML/tinyxml.h"
#include <windows.h>
#include <tchar.h>

#ifndef WINVER				// 允许使用特定于 Windows XP 或更高版本的功能。
#define WINVER 0x0501		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0410 // 将此值更改为适当的值，以指定将 Windows Me 或更高版本作为目标。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 6.0 或更高版本的功能。
#define _WIN32_IE 0x0600	// 将此值更改为相应的值，以适用于 IE 的其他版本。
#endif

#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料

namespace Flagship
{
	using namespace std;
	using namespace Wm4;

	// 实体过期时间
    #define ENTITY_TIME        30000
	// 数据中心缓冲
    #define DATACENTER_CACHE   5
	// 单元大小
    #define CELL_SIZE          64
	// 单元视距
    #define CELL_VIEW          4
	// 单元高度
    #define CELL_HEIGHT        64
	// 逻辑帧
    #define LOGIC_FPS          30
	// 最大光源数
    #define MAX_LIGHT          8

	// 资源加载消息
    #define WM_LOADRESOURCE    WM_USER + 1
	// 添加实体消息
    #define WM_ADDENTITY       WM_USER + 2
	// 更新实体消息
    #define WM_UPDATEENTITY    WM_USER + 3
	// 删除实体消息
    #define WM_DELENTITY       WM_USER + 4

	// 关闭线程安全
//  #define _HAS_ITERATOR_DEBUGGING 0
//  #define _SECURE_SCL 0

	// 平方宏
    #define SQR(x)    ( ( x ) * ( x ) )

	// 安全释放宏
	#ifndef SAFE_DELETE
	#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
	#endif    
	#ifndef SAFE_DELETE_ARRAY
	#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
	#endif    
	#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
	#endif

	#ifndef V
	#define V(x)           { hr = (x); }
	#endif
	#ifndef V_RETURN
	#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return hr; } }
	#endif
}

#endif