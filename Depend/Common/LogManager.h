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

		// ���Log������ָ��
		static LogManager *    GetSingleton();

		//  ��¼Log
		void                   WriteLog( const char * szLog );

	protected:
		// ��ǰLog�ļ�
		FILE *                 m_pFile;
		
	private:
		LogManager();

		static LogManager *    m_pLogManager;
	};
}

#endif