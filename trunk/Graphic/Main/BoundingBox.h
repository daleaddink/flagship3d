#ifndef _BOUNDINGBOX_H_
#define _BOUNDINGBOX_H_

#include <Common/Prerequisites.h>
#include "BoundingVolume.h"

namespace Flagship
{
	class _DLL_Export BoundingBox : public BoundingVolume
	{
	public:
		BoundingBox();
		BoundingBox( Box3f& rkBox );
		BoundingBox( Vector3f& vCenter, Vector3f * avAxis = NULL, float * afExtent = NULL );
		virtual ~BoundingBox();

		// ���ص���
		BoundingBox&         operator = ( BoundingBox& rkBound );

		// ��������
		void                 SetCenter( Vector3f& rvCenter ) { m_kBox.Center = rvCenter; }

		// ��������
		void                 SetAxis( Vector3f * avAxis );

		// ������չ
		void                 SetExtent( float * afExtent );

		// ��ȡ����
		Box3f&               GetBox() { return m_kBox; }

	public:
		// �������
		virtual void         GrowToContain( BoundingVolume * pBound );

		// �ж�λ��
		virtual bool         IsInBound( Vector3f& vPos );

		// �Ƿ񱻼���
		virtual bool         IsCulled( Plane3f * pkPlane );

		// �Ƿ��ཻ
		virtual bool         IsIntersect( BoundingVolume * pBound );

		// ����任
		virtual void         TransformBy( Matrix4f& rmatWorld, BoundingVolume * pkResultBound );

		// ��ȡ����ƽ��
		virtual Plane3f *    GetBackPlane( Vector3f& vPos );

	protected:
		// ����
		Box3f           m_kBox;

	private:
	};
}

#endif
