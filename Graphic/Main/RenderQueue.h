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

		// ������������
		void                             SetSortType( int iType );

		// ����ʵ��
		void                             InsertEntity( Entity * pEntity );

		// ����ʵ��
		void                             InsertEntity( Vector4f& vRefer, Entity * pEntity );

		// ���ʵ���б�
		multimap< float, Entity * > *    GetEntityList();

		// �����Ⱦ����
		void                             Clear();

	protected:
		// ��������
		int                              m_iSortType;

		// ��Ⱦ����
		multimap< float, Entity * >      m_pRenderList;
		
	private:
	};
}

#endif