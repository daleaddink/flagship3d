#include <Common/Prerequisites.h>
#include <Common/LogManager.h>

#include "Resource.h"
#include "ResourceManager.h"
#include "Entity.h"
#include "Renderer.h"

namespace Flagship
{
	ResourceManager * ResourceManager::m_pResourceManager = NULL;

	ResourceManager::ResourceManager()
	{
		m_pResourceMap.clear();
		m_pCacheList.clear();
		m_pCacheIterator = m_pCacheList.begin();
	}

	ResourceManager::~ResourceManager()
	{

	}

	ResourceManager *    ResourceManager::GetSingleton()
	{
		if ( m_pResourceManager == NULL )
		{
			m_pResourceManager = new ResourceManager;
		}

		return m_pResourceManager;
	}

	Resource *    ResourceManager::GetResource( Key& kKey )
	{
		map<Key, Resource *>::iterator it = m_pResourceMap.find( kKey );
		if ( it != m_pResourceMap.end() )
		{
			return (*it).second;
		}

		return NULL;		
	}

	void    ResourceManager::AddResource( Resource * pResource )
	{
		map<Key, Resource *>::iterator it = m_pResourceMap.find( pResource->GetKey() );
		if ( it != m_pResourceMap.end() )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, pResource->GetKey().GetName().c_str(), 256 );
			sprintf( szLog, "ResourceManager::AddResource Key Duplicate! Resource:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );
			return;
		}

		m_pResourceMap[pResource->GetKey()] = pResource;
	}

	void    ResourceManager::DelResource( Resource * pResource )
	{
		m_pResourceMap.erase( pResource->GetKey() );
	}

	void    ResourceManager::Update()
	{
		if ( m_pCacheIterator != m_pCacheList.end() )
		{
			if ( ( * m_pCacheIterator )->IsOutOfDate() )
			{
				// 资源已超时
				if ( ( * m_pCacheIterator )->IsReady() )
				{
//					( * m_pCacheIterator )->UnCache();
				}

				m_pCacheIterator = m_pCacheList.erase( m_pCacheIterator );
			}
			else
			{
				m_pCacheIterator++;
			}
		}
		else
		{
			m_pCacheIterator = m_pCacheList.begin();
		}
	}
}