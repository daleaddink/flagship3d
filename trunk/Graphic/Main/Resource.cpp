#include <Common/Prerequisites.h>

#include "Resource.h"
#include "RenderWindow.h"

namespace Flagship
{
	Resource::Resource()
	{
		m_iClassType = Base::Class_Resource;
		m_bReady = false;
		m_szPathName = L"Terrain";
		m_dwLastFrame = 0;
		m_iRefCount = 0;
	}

	Resource::~Resource()
	{
		
	}

	bool    Resource::IsReady()
	{
		return m_bReady;
	}

	bool    Resource::IsOutOfDate()
	{
		if ( m_dwLastFrame == 0 )
		{
			return false;
		}

		return ( RenderWindow::GetActiveRenderWindow()->GetCurrentFrame() - m_dwLastFrame ) > ENTITY_TIME;
	}

	void    Resource::UpdateFrame()
	{
		// 设置最近渲染时间
		m_dwLastFrame = RenderWindow::GetActiveRenderWindow()->GetCurrentFrame();
	}

	void    Resource::SetPath( wstring szPath )
	{
		m_szPathName = szPath;
		m_szKey.SetName( szPath );
	}

	bool    Resource::Load()
	{
		m_iRefCount++;

		if ( m_bReady )
		{
			return false;
		}

		if ( GetClassType() != Base::Mesh_Terrain )
		{
			if ( m_szPathName == L"" )
			{
				return false;
			}

			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			FILE * pFile = fopen( szFile, "rb" );

			if ( pFile == NULL )
			{
				return false;
			}

			DWORD dwSize = _filelength( _fileno( pFile ) );
			m_kFileBuffer.Create( dwSize );
			fread( m_kFileBuffer.GetPointer(), sizeof(char), dwSize, pFile );
		}

		if ( ! Create() )
		{
			return false;
		}

		if ( ! Cache() )
		{
			return false;
		}
		
		m_bReady = true;
		return true;
	}

	bool    Resource::Release()
	{
		if ( m_iRefCount > 0 )
		{
			m_iRefCount--;
			if ( m_iRefCount == 0 )
			{
				ResourceManager::GetSingleton()->DelResource( this );
			}

			return false;
		}

		UnCache();
		Destory();
		m_kFileBuffer.Release();
		m_bReady = false;

		return true;
	}
}