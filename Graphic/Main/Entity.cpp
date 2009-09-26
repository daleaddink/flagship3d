#include <Common/LogManager.h>

#include "Entity.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "RenderWindow.h"
#include "Renderer.h"
#include "Resource.h"
#include "Mesh.h"
#include "ResourceLoader.h"
#include "Cell.h"
#include "Light.h"
#include "Material.h"
#include "DataCenter.h"
#include "GlobalParam.h"

#define m_pEntityData ( (EntityData *) m_pSharedData )

namespace Flagship
{
	Entity::Entity( wstring szName )
	{
		m_iClassType = Base::Class_Entity;
		m_dwLastFrame = 0;
		m_pCullMesh = NULL;
		m_pParentCell = NULL;

		m_bAffectByLight = true;
		m_bCastShadow = true;
		m_bReceiveShadow = true;

		// 设置名称
		if ( szName != L"" )
		{
			SetName( szName );
		}

		// 设置包围体
		m_pBound = new BoundingBox;
		m_pLocalBound = new BoundingBox;
		* m_pLocalBound = * ( (BoundingBox *) m_pBound );
	}

	Entity::~Entity()
	{
		Release();
		SAFE_DELETE( m_pBound );
		SAFE_DELETE( m_pLocalBound );
	}

	bool    Entity::IsReady()
	{
		list< Resource * >::iterator it;
		for ( it = m_pResourceList.begin(); it != m_pResourceList.end(); it++ )
		{
			bool bReady = (*it)->IsReady();
			if ( ! bReady )
			{
				return false;
			}
		}

		map<Key, Material *>::iterator MatIt = m_pMaterialMap.begin();
		while ( MatIt != m_pMaterialMap.end() )
		{
			if ( ! (*MatIt).second->IsReady() )
			{
				return false;
			}

			MatIt++;
		}

		return true;
	}

	void    Entity::SetOuterBound( BoundingVolume * pBound )
	{
		if ( pBound->GetBoundType() == BoundingVolume::Bounding_Box )
		{
			* ( (BoundingBox *) m_pLocalBound ) = * ( (BoundingBox *) pBound );
		}
		else if ( pBound->GetBoundType() == BoundingVolume::Bounding_Sphere )
		{
			* ( (BoundingSphere *) m_pLocalBound ) = * ( (BoundingSphere *) pBound );
		}
	}

	void    Entity::SetInnerBound( Mesh * pCullMesh )
	{
		m_pCullMesh = pCullMesh;
	}

	void    Entity::UpdateScene()
	{
		Matrix4f matWorldMatrix = * ( ( (MatrixData *) m_pEntityData->m_pData[EntityData::Matrix4_World] )->Read() );
		m_pLocalBound->TransformBy( matWorldMatrix, m_pBound );

		// 取位置
		Vector4f vMatPos = matWorldMatrix.GetColumn( 3 );
		Vector3f vPos( vMatPos[0], vMatPos[1], vMatPos[2] );

		if ( m_pParentCell != NULL )
		{			
			if ( m_pParentCell->GetBound()->IsInBound( vPos ) )
			{
				return;
			}

			// 更新单元
			Cell * pNewCell = SceneManager::GetSingleton()->GetCell( vPos );
			m_pParentCell->DetachEntity( this );
			m_pParentCell = pNewCell;
			m_pParentCell->AttechEntity( this );
		}
		else
		{
			// 更新单元
			Cell * pNewCell = SceneManager::GetSingleton()->GetCell( vPos );
			m_pParentCell = pNewCell;
			m_pParentCell->AttechEntity( this );
		}
	}

	void    Entity::UpdateFrame()
	{
		m_dwLastFrame = RenderWindow::GetActiveRenderWindow()->GetCurrentFrame();
	}

	int    Entity::GetMaterialType()
	{
		map<Key, Material *>::iterator it = m_pMaterialMap.begin();
		while ( it != m_pMaterialMap.end() )
		{
			if ( (*it).second->GetClassType() == Base::Material_RenderTexture )
			{
				return Base::Material_RenderTexture;
			}

			it++;
		}

		return Base::Material_General;
	}

	void    Entity::SetParent( Cell * pParent )
	{
		m_pParentCell = pParent;
	}

	Cell *    Entity::GetParent()
	{
		return m_pParentCell;
	}

	void    Entity::AttachResource( Resource * pResource, Material * pMaterial )
	{
		if ( m_pMaterialMap.find( pResource->GetKey() ) != m_pMaterialMap.end() )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, pResource->GetKey().GetName().c_str(), 256 );
			sprintf( szLog, "Entity::AttachResource Key Duplicate! Resource:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );
			return;
		}

		m_pResourceList.push_back( pResource );
		m_pMaterialMap[pResource->GetKey()] = pMaterial;
	}
	
	void    Entity::SetWorldMatrix( Matrix4f& matWorldMatrix )
	{
		( (MatrixData *) m_pEntityData->m_pData[EntityData::Matrix4_World] )->Write( matWorldMatrix );
   		PostThreadMessage( SceneManager::GetSingleton()->GetThreadID(), WM_UPDATEENTITY, (WPARAM) this, 0 );
	}

	void    Entity::Render( Renderer * pRenderer, BYTE byTechType )
	{
		Material * pGlobalMaterial = GlobalParam::GetSingleton()->m_pGlobalMaterial;

		// 渲染资源
		list< Resource * >::iterator it;
		for ( it = m_pResourceList.begin(); it != m_pResourceList.end(); it++ )
		{
			map<Key, Material *>::iterator MatIt = m_pMaterialMap.find( (*it)->GetKey() );
			if ( MatIt != m_pMaterialMap.end() )
			{
				if ( (*MatIt).second->SetTechnique( this, byTechType ) )
				{
					(*MatIt).second->Update( this );
    				pRenderer->RenderSingle( (*MatIt).second, (*it) );
				}
				else if ( pGlobalMaterial != NULL )
				{
					if ( pGlobalMaterial->SetTechnique( this, byTechType ) )
					{
						pGlobalMaterial->Update( this );
						pRenderer->RenderSingle( pGlobalMaterial, (*it) );
					}
				}
				
				(*it)->UpdateFrame();
			}
			else
			{
				char szLog[10240];
				char szFile[256];
				wcstombs( szFile, (*it)->GetKey().GetName().c_str(), 256 );
				sprintf( szLog, "Entity::Render Material Not Find! Resource:%s", szFile );
				LogManager::GetSingleton()->WriteLog( szLog );
			}
		}
	}

	bool    Entity::Load()
	{
		list< Resource * >::iterator it;
		for ( it = m_pResourceList.begin(); it != m_pResourceList.end(); it++ )
		{
			ResourceLoader::GetSingleton()->PushResource( (*it) );
		}

		map<Key, Material *>::iterator MatIt;
		for ( MatIt = m_pMaterialMap.begin(); MatIt != m_pMaterialMap.end(); MatIt++ )
		{
			(*MatIt).second->Load();
		}

		if ( m_pCullMesh != NULL )
		{
			ResourceLoader::GetSingleton()->PushResource( m_pCullMesh );
		}

		return true;
	}

	bool    Entity::Release()
	{
		list< Resource * >::iterator it;
		for ( it = m_pResourceList.begin(); it != m_pResourceList.end(); it++ )
		{
			(*it)->Release();
		}

		map<Key, Material *>::iterator MatIt;
		for ( MatIt = m_pMaterialMap.begin(); MatIt != m_pMaterialMap.end(); MatIt++ )
		{
			(*MatIt).second->Release();
		}

		if ( m_pCullMesh != NULL )
		{
    		m_pCullMesh->Release();
		}

		return true;
	}
}
