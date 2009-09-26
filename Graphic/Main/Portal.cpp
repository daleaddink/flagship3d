#include "Portal.h"
#include "SceneNode.h"
#include "Renderer.h"
#include "BoundingBox.h"
#include "SceneManager.h"

#include "DataDefine.h"
#include "DataCenter.h"

#define m_pPortalData ( (PortalData *) m_pSharedData )

namespace Flagship
{
	Portal::Portal( wstring& szName, BoundingVolume * pBound, Matrix4f * pWorld )
	{
		m_iClassType = Base::Class_Portal;

		// 设置名称
		if ( szName != L"" )
		{
			SetName( szName );
		}

		// 设置包围体
		m_pBound = new BoundingBox;
		if ( pBound != NULL )
		{
			* m_pBound = * ( (BoundingBox *) pBound );
		}
	}

	Portal::~Portal()
	{
		SAFE_DELETE( m_pBound );
	}

	void    Portal::SetOpen( bool bOpen )
	{
		m_bOpen = bOpen;
	}

	bool    Portal::GetOpen()
	{
		return m_bOpen;
	}

	void    Portal::LinkSceneNode( SceneNode * pInnerNode, SceneNode * pOuterNode )
	{
		m_pInnerNode = pInnerNode;
		m_pOuterNode = pOuterNode;
	}

	SceneNode *    Portal::GetOppositeNode( SceneNode * pNode )
	{
		if ( pNode == m_pInnerNode )
		{
			return m_pOuterNode;
		}

		if ( pNode == m_pOuterNode )
		{
			return m_pInnerNode;
		}

		return NULL;
	}

	void    Portal::SetWorldMatrix( Matrix4f& matWorldMatrix )
	{
		( (MatrixData *) m_pPortalData->m_pData[PortalData::Matrix4_World] )->Write( matWorldMatrix );
		PostThreadMessage( SceneManager::GetSingleton()->GetThreadID(), WM_UPDATEENTITY, (WPARAM) this, 0 );
	}
}