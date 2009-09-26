#include <Common/Prerequisites.h>

#include "ResourceLoader.h"

namespace Flagship
{
	ResourceLoader * ResourceLoader::m_pResourceLoader = NULL;

	ResourceLoader::ResourceLoader()
	{
		m_pResourceList.clear();
    }

	ResourceLoader::~ResourceLoader()
	{

	}

	ResourceLoader *    ResourceLoader::GetSingleton()
	{
		if ( m_pResourceLoader == NULL )
		{
			m_pResourceLoader = new ResourceLoader;
		}

		return m_pResourceLoader;
	}

	void    ResourceLoader::PushResource( Resource * pResource )
	{
		PostThreadMessage( GetThreadID(), WM_LOADRESOURCE, (WPARAM) pResource, 0 );
	}

	int     ResourceLoader::Run()
	{
		MSG msg;
		PeekMessage( &msg, NULL, WM_USER, WM_USER, PM_NOREMOVE );

		while ( m_bRunning )
		{
			if ( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
			{
				if ( ! GetMessage( &msg, NULL, 0, 0 ) )
				{
					return (int) msg.wParam;
				}

				( (Resource *) msg.wParam )->Load();
			}
			else
			{
				WaitMessage();
			}
		}

		return 0;
	}
}