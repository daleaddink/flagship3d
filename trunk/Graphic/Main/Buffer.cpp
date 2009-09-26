#include "Buffer.h"
#include <Common/LogManager.h>

namespace Flagship
{
	Buffer::Buffer()
	{
		m_pBuffer = NULL;
		m_dwSize = 0;
	}

	Buffer::~Buffer()
	{
		SAFE_DELETE_ARRAY( m_pBuffer );
	}

	bool    Buffer::Create( DWORD dwSize )
	{
		m_pBuffer = new char[dwSize];
		m_dwSize = dwSize;

		return true;
	}

	bool    Buffer::Copy( char * pBuffer, DWORD dwSize )
	{
		if ( dwSize > m_dwSize )
		{
			char szLog[10240];
			sprintf( szLog, "Buffer::Copy() Fail! Source Too Large!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		memcpy( m_pBuffer, pBuffer, dwSize );
		return true;
	}

	void    Buffer::SetPointer( void * pData )
	{
		m_pBuffer = (char *) pData;
	}

	void *    Buffer::GetPointer()
	{
		return (void *) m_pBuffer;
	}

	void    Buffer::SetSize( DWORD dwSize )
	{
		m_dwSize = dwSize;
	}

	DWORD    Buffer::GetSize()
	{
		return m_dwSize;
	}

	void    Buffer::Release()
	{
		SAFE_DELETE( m_pBuffer );
	}
}