#ifndef _SECTORNODE_H_
#define _SECTORNODE_H_

#include <Common/Prerequisites.h>
#include "Cell.h"

namespace Flagship
{
	class Portal;
	class Entity;
	class Culler;
	struct SectorNodeData;

	class _DLL_Export SectorNode : public Cell
	{
	public:
		SectorNode( wstring szName = L"" );
		virtual ~SectorNode();

		// 设置BSP平面面
		void                      SetBSPlane( Plane3f& rkBSPlane );

		// 获取BSP平面面
		Plane3f *                 GetBSPlane() { return &m_kBSPlane; }

		// 获取入口列表
		map< Key, Portal * > *    GetPortalMap();

	public:
		// 设置世界矩阵
		virtual void              SetWorldMatrix( Matrix4f& matWorldMatrix );

		// 添加实体
		virtual void              AttechEntity( Entity * pEntity );

		// 删除实体
		virtual bool              DetachEntity( Entity * pEntity );

	protected:
		// 共享数据
		SectorNodeData *          m_pSectorNodeData;

		// 入口列表
		map< Key, Portal * >      m_pPortalMap;

		// BSP平面
		Plane3f                   m_kBSPlane;

	private:
	};
}

#endif