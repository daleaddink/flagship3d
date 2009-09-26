#ifndef _RENDERQUEUE_H_
#define _RENDERQUEUE_H_

#include <Common/Prerequisites.h>
#include "Entity.h"

namespace Flagship
{
	class Entity;
	class _DLL_Export RenderQueue
	{
	public:
		RenderQueue();
		virtual ~RenderQueue();

		enum
		{
			Sort_FrontToBack,
			Sort_BackToFront,
			Sort_ShadowCaster,
			Sort_None,
		};

		// 设置排序类型
		void                             SetSortType( int iType );

		// 插入实体
		void                             InsertEntity( Entity * pEntity );

		// 插入实体
		void                             InsertEntity( Vector4f& vRefer, Entity * pEntity );

		// 获得实体列表
		multimap< float, Entity * > *    GetEntityList();

		// 清除渲染队列
		void                             Clear();

	protected:
		// 排序类型
		int                              m_iSortType;

		// 渲染队列
		multimap< float, Entity * >      m_pRenderList;
		
	private:
	};
}

#endif