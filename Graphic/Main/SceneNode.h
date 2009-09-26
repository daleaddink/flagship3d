#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#include <Common/Prerequisites.h>
#include "Renderable.h"
#include "BoundingBox.h"

namespace Flagship
{
	class Entity;
	class Culler;
	struct SceneNodeData;

	class _DLL_Export SceneNode : public Renderable
	{
	public:
		SceneNode( wstring szName = L"" );
		virtual ~SceneNode();

		// 设置包围体
		void                      SetBound( BoundingBox * pBound );

		// 设置父节点
		void                      SetParent( SceneNode * pParent );

		// 获取父节点
		SceneNode *               GetParent();

		// 获取子节点列表
		map< Key, SceneNode * > * GetChildMap();

		// 添加子节点
		void                      AttechChild( SceneNode * pChild );

		// 清空子节点
		void                      ClearChild();

		// 更新包围盒
		void                      UpdateBound( Vector2f& vOffset );

		// 获取实体列表
		map< Key, Entity * > *    GetEntityMap() { return &m_pEntityMap; }

	public:
		// 添加实体
		virtual void              AttechEntity( Entity * pEntity ) {}

		// 删除实体
		virtual bool              DetachEntity( Entity * pEntity ) { return true; }

	protected:
		// 共享数据
		SceneNodeData *           m_pSceneNodeData;

		// 父节点
		SceneNode *               m_pParentNode;

		// 子节点列表
		map< Key, SceneNode * >   m_pChildMap;

		// 实体列表
		map< Key, Entity * >      m_pEntityMap;

		// 剪裁体计数
		int                       m_iCullNum;

	private:
	};
}

#endif