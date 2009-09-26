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
		// 扩大包容
		virtual void         GrowToContain( BoundingVolume * pBound ) = 0;

		// 判断位置
		virtual bool         IsInBound( Vector3f& vPos ) = 0;

		// 是否被剪裁
		virtual bool         IsCulled( Plane3f * pkPlane ) = 0;

		// 是否相交
		virtual bool         IsIntersect( BoundingVolume * pBound ) = 0;

		// 坐标变换
		virtual void         TransformBy( Matrix4f& rmatWorld, BoundingVolume * pkResultBound ) = 0;

		// 获取背面平面
		virtual Plane3f *    GetBackPlane( Vector3f& vPos ) = 0;

	protected:
        // 包围体类型
		int             m_iBoundType;
		
	private:
	};
}

#endif
