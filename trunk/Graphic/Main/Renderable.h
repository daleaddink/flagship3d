#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include <Common/Prerequisites.h>
#include "Base.h"

namespace Flagship
{
	class Material;
	class BoundingVolume;

	class _DLL_Export Renderable : public Base
	{
	public:
		Renderable();
		virtual ~Renderable();

		// ��ȡ��Χ��
		BoundingVolume *    GetBound() { return m_pBound; }

		// ��ȡע������
		void *              GetSharedData() { return m_pSharedData; }

	public:
		// ��������
		virtual void        SetName( wstring& szName );

		// �Զ���������
		virtual void        AutoName();

		// �����������
		virtual void        SetWorldMatrix( Matrix4f& matWorldMatrix ) {}

		// �Ƿ����
		virtual bool        IsReady() { return false; }

		// ��ȡ��Դ
		virtual bool        Load() { return true; }

		// �ͷ���Դ
		virtual bool        Release() { return true; }

	protected:
		// ��Χ��
		BoundingVolume *    m_pBound;

		// ��������
		void *              m_pSharedData;

	private:
	};
}

#endif