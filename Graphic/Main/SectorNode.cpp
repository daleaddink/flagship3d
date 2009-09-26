#include "SectorNode.h"
#include "BoundingBox.h"
#include "Entity.h"
#include "Culler.h"
#include "SceneManager.h"

#include "DataDefine.h"
#include "DataCenter.h"

#define m_pSectorNodeData ( (SectorNodeData *) m_pSharedData )

namespace Flagship
{
	SectorNode::SectorNode( wstring szName )
	{
		m_iClassType = Base::SceneNode_Sector;

		// 设置名称
		if ( szName != L"" )
		{
			SetName( szName );
		}

		// 设置包围体
		m_pBound = new BoundingBox;
	}

	SectorNode::~SectorNode()
	{

	}

	void    SectorNode::SetBSPlane( Plane3f& rkBSPlane )
	{
		m_kBSPlane = rkBSPlane;
	}

	map< Key, Portal * > *    SectorNode::GetPortalMap()
	{
		return &m_pPortalMap;
	}

	void    SectorNode::SetWorldMatrix( Matrix4f& matWorldMatrix )
	{
		( (MatrixData *) m_pSectorNodeData->m_pData[SectorNodeData::Matrix4_World] )->Write( matWorldMatrix );
		PostThreadMessage( SceneManager::GetSingleton()->GetThreadID(), WM_UPDATEENTITY, (WPARAM) this, 0 );
	}

	void    SectorNode::AttechEntity( Entity * pEntity )
	{
		// 添加入口
		if ( pEntity->GetClassType() == Base::Class_Portal	)
		{
			m_pPortalMap[ pEntity->GetKey() ] = (Portal *) pEntity;
		}

		Cell::AttechEntity( pEntity );
	}

	bool    SectorNode::DetachEntity( Entity * pEntity )
	{
		// 删除入口
		if ( pEntity->GetClassType() == Base::Class_Portal )
		{
			map< Key, Portal * >::iterator it = m_pPortalMap.find( pEntity->GetKey() );
			if ( it == m_pPortalMap.end() )
			{
				return false;
			}

			m_pPortalMap.erase( it );
		}

		return Cell::DetachEntity( pEntity );
	}
}