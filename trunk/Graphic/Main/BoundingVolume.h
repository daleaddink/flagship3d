#ifndef _BOUND_H_
#define _BOUND_H_

#include <Common/Prerequisites.h>

namespace Flagship
{
	class _DLL_Export BoundingVolume
	{
	public:
		BoundingVolume();
		virtual ~BoundingVolume();

		enum
		{
			Bounding_Box,
			Bounding_Sphere,
			Bounding_Max,
		};

		int                  GetBoundType() { return m_iBoundType; }

	public:
		// �������
		virtual void         GrowToContain( BoundingVolume * pBound ) = 0;

		// �ж�λ��
		virtual bool         IsInBound( Vector3f& vPos ) = 0;

		// �Ƿ񱻼���
		virtual bool         IsCulled( Plane3f * pkPlane ) = 0;

		// �Ƿ��ཻ
		virtual bool         IsIntersect( BoundingVolume * pBound ) = 0;

		// ����任
		virtual void         TransformBy( Matrix4f& rmatWorld, BoundingVolume * pkResultBound ) = 0;

		// ��ȡ����ƽ��
		virtual Plane3f *    GetBackPlane( Vector3f& vPos ) = 0;

	protected:
        // ��Χ������
		int             m_iBoundType;
		
	private:
	};
}

#endif
