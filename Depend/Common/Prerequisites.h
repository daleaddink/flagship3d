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

#ifndef WINVER				// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define WINVER 0x0501		// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0410 // ����ֵ����Ϊ�ʵ���ֵ����ָ���� Windows Me ����߰汾��ΪĿ�ꡣ
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 6.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0600	// ����ֵ����Ϊ��Ӧ��ֵ���������� IE �������汾��
#endif

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����

namespace Flagship
{
	using namespace std;
	using namespace Wm4;

	// ʵ�����ʱ��
    #define ENTITY_TIME        30000
	// �������Ļ���
    #define DATACENTER_CACHE   5
	// ��Ԫ��С
    #define CELL_SIZE          64
	// ��Ԫ�Ӿ�
    #define CELL_VIEW          4
	// ��Ԫ�߶�
    #define CELL_HEIGHT        64
	// �߼�֡
    #define LOGIC_FPS          30
	// ����Դ��
    #define MAX_LIGHT          8

	// ��Դ������Ϣ
    #define WM_LOADRESOURCE    WM_USER + 1
	// ���ʵ����Ϣ
    #define WM_ADDENTITY       WM_USER + 2
	// ����ʵ����Ϣ
    #define WM_UPDATEENTITY    WM_USER + 3
	// ɾ��ʵ����Ϣ
    #define WM_DELENTITY       WM_USER + 4

	// �ر��̰߳�ȫ
//  #define _HAS_ITERATOR_DEBUGGING 0
//  #define _SECURE_SCL 0

	// ƽ����
    #define SQR(x)    ( ( x ) * ( x ) )

	// ��ȫ�ͷź�
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