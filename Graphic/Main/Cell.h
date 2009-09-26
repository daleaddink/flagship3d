#ifndef _CELL_H_
#define _CELL_H_

#include <Common/Prerequisites.h>
#include "SceneNode.h"

namespace Flagship
{
	class Entity;

	class _DLL_Export Cell : public SceneNode
	{
	public:
		Cell();
		virtual ~Cell();

		// 设置索引
		void                      SetIndex( int iIndexX, int iIndexZ );

		// 获取X索引
		int                       GetIndexX() { return m_iIndexX; }

		// 获取Z索引
		int                       GetIndexZ() { return m_iIndexZ; }

	public:
		// 加载单元
		virtual bool              Load();

		// 卸载单元
		virtual bool              Release();

		// 添加实体
		virtual void              AttechEntity( Entity * pEntity );

		// 删除实体
		virtual bool              DetachEntity( Entity * pEntity );

	protected:
		// 索引
		int                       m_iIndexX;
		int                       m_iIndexZ;
	};
}

#endif
