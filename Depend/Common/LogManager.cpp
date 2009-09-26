#include "LogManager.h"

namespace Flagship
{
	LogManager * LogManager::m_pLogManager = NULL;

	LogManager::LogManager()
	{
		m_pFile = fopen("Log.txt", "w");
	}

	LogManager::~LogManager()
	{
		fclose( m_pFile );
		SAFE_DELETE( m_pLogManager );
	}

	LogManager *    LogManager::GetSingleton()
	{
		if ( m_pLogManager == NULL )
		{
			m_pLogManager = new LogManager;
		}

		return m_pLogManager;
	}

	void    LogManager::WriteLog( const char * szLog )
	{
		char szData[128];
		_strdate( szData );
		char szTime[128];
		_strtime( szTime );
		
		char szTemp[10240];
		sprintf( szTemp, "%s %s : %s", szData, szTime, szLog );

		fwrite( szTemp, sizeof(char), strlen(szTemp), m_pFile );
		fwrite( "\r\n", sizeof(char), strlen("\r\n"), m_pFile );
	}
}

	