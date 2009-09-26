#ifndef _LOGMANAGER_H_
#define _LOGMANAGER_H_

#include "Prerequisites.h"
#include <wchar.h>

namespace Flagship
{
	class _DLL_Export LogManager
	{
	public:
		virtual ~LogManager();

		// 获得Log管理器指针
		static LogManager *    GetSingleton();

		//  记录Log
		void                   WriteLog( const char * szLog );

	protected:
		// 当前Log文件
		FILE *                 m_pFile;
		
	private:
		LogManager();

		static LogManager *    m_pLogManager;
	};
}

#endif