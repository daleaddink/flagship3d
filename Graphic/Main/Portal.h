#ifndef _PORTAL_H_
#define _PORTAL_H_

#include <Common/Prerequisites.h>
#include "Renderable.h"

namespace Flagship
{
	class  SceneNode;
	struct PortalData;
	
	class _DLL_Export Portal : public Renderable
	{
	public:
		Portal( wstring& szName, BoundingVolume * pBound = NULL, Matrix4f * pWorld = NULL );
		virtual ~Portal();

		// 设置开关状态
		void                           SetOpen( bool bOpen );

		// 获取开关状态
		bool                           GetOpen();

		// 连接场景节点
		void                           LinkSceneNode( SceneNode * pInnerNode, SceneNode * pOuterNode );

		// 获取对面节点
		SceneNode *                    GetOppositeNode( SceneNode * pNode );

	public:
		// 设置世界矩阵
		virtual void                   SetWorldMatrix( Matrix4f& matWorldMatrix );

	protected:
		// 共享数据
		PortalData *                   m_pPortalData;

		//  开关状态
		bool                           m_bOpen;

		// 内部节点
		SceneNode *                    m_pInnerNode;

		// 外部节点
		SceneNode *                    m_pOuterNode;
		
	private:

	};
}

#endif