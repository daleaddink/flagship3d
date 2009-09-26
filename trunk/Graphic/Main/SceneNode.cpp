#include "SceneNode.h"
#include "Entity.h"
#include "Culler.h"

#include "DataDefine.h"
#include "DataCenter.h"

#define m_pSceneNodeData ( (SceneNodeData *) m_pSharedData )

namespace Flagship
{
	SceneNode::SceneNode( wstring szName )
	{
		m_iClassType = Base::Class_SceneNode;
		m_iCullNum = 0;

		// 设置名称
		if ( szName != L"" )
		{
			SetName( szName );
		}

		// 设置包围体
		m_pBound = new BoundingBox;
	}

	SceneNode::~SceneNode()
	{
		map< Key, SceneNode * >::iterator it = m_pChildMap.begin();
		while ( it != m_pChildMap.end() )
		{
			SAFE_DELETE( (*it).second );
		}

		m_pChildMap.clear();
	}

	void    SceneNode::SetBound( BoundingBox * pBound )
	{
		* ( (BoundingBox *) m_pBound ) = * pBound;
	}

	void    SceneNode::SetParent( SceneNode * pParent )
	{
		m_pParentNode = pParent;
	}

	map< Key, SceneNode * > *    SceneNode::GetChildMap() 
	{
		return &m_pChildMap;
	}
	
	SceneNode *    SceneNode::GetParent()
	{
		return m_pParentNode;
	}

	void    SceneNode::AttechChild( SceneNode * pChild )
	{
		m_pChildMap[ pChild->GetKey() ] = pChild;
	}

	void    SceneNode::ClearChild()
	{
		m_pChildMap.clear();
	}

	void    SceneNode::UpdateBound( Vector2f& vOffset )
	{
		Vector3f vCenter = ( (BoundingBox *) m_pBound )->GetBox().Center;
		( (BoundingBox *) m_pBound )->SetCenter( Vector3f( vCenter.X() + vOffset.X(), vCenter.Y(), vCenter.Z() + vOffset.Y() ) );

		map< Key, SceneNode * >::iterator ChildIt = m_pChildMap.begin();
		while ( ChildIt != m_pChildMap.end() )
		{
			(*ChildIt).second->UpdateBound( vOffset );
			ChildIt++;
		}
	}
}
