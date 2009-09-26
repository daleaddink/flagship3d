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

		// ��������
		void                      SetIndex( int iIndexX, int iIndexZ );

		// ��ȡX����
		int                       GetIndexX() { return m_iIndexX; }

		// ��ȡZ����
		int                       GetIndexZ() { return m_iIndexZ; }

	public:
		// ���ص�Ԫ
		virtual bool              Load();

		// ж�ص�Ԫ
		virtual bool              Release();

		// ���ʵ��
		virtual void              AttechEntity( Entity * pEntity );

		// ɾ��ʵ��
		virtual bool              DetachEntity( Entity * pEntity );

	protected:
		// ����
		int                       m_iIndexX;
		int                       m_iIndexZ;
	};
}

#endif
