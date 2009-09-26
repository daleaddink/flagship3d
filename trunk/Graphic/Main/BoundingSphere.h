#ifndef _BOUNDINGSPHERE_H_
#define _BOUNDINGSPHERE_H_

#include <Common/Prerequisites.h>
#include "BoundingVolume.h"

namespace Flagship
{
	class _DLL_Export BoundingSphere : public BoundingVolume
	{
	public:
		BoundingSphere();
		BoundingSphere( Sphere3f& rkSphere );
		BoundingSphere( Vector3f& vCenter, float fRadius = 0 );
		virtual ~BoundingSphere();

		// ���ص���
		BoundingSphere&      operator = ( BoundingSphere& rkBound );

		// ��������
		void                 SetCenter( Vector3f& rkCenter ) { m_kSphere.Center = rkCenter; }

		// ���ð�Χ��뾶
		void                 SetRadius( float fRadius ) { m_kSphere.Radius = fRadius; }

		// ��ȡ��
		Sphere3f&            GetSphere() { return m_kSphere; }

	public:
        // �������
		virtual void         GrowToContain( BoundingVolume * pBound );

		// �ж�λ��
		virtual bool         IsInBound( Vector3f& rvPos );

		// �Ƿ񱻼���
		virtual bool         IsCulled( Plane3f * pkPlane );

		// �Ƿ��ཻ
		virtual bool         IsIntersect( BoundingVolume * pBound );

		// ����任
		virtual void         TransformBy( Matrix4f& rmatWorld, BoundingVolume * pkResultBound );

		// ��ȡ����ƽ��
		virtual Plane3f *    GetBackPlane( Vector3f& vPos );

	protected:
		// ��
		Sphere3f             m_kSphere;

	private:
	};
}

#endif
