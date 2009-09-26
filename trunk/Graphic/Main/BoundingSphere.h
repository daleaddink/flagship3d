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

		// 重载等于
		BoundingSphere&      operator = ( BoundingSphere& rkBound );

		// 设置中心
		void                 SetCenter( Vector3f& rkCenter ) { m_kSphere.Center = rkCenter; }

		// 设置包围球半径
		void                 SetRadius( float fRadius ) { m_kSphere.Radius = fRadius; }

		// 获取球
		Sphere3f&            GetSphere() { return m_kSphere; }

	public:
        // 扩大包容
		virtual void         GrowToContain( BoundingVolume * pBound );

		// 判断位置
		virtual bool         IsInBound( Vector3f& rvPos );

		// 是否被剪裁
		virtual bool         IsCulled( Plane3f * pkPlane );

		// 是否相交
		virtual bool         IsIntersect( BoundingVolume * pBound );

		// 坐标变换
		virtual void         TransformBy( Matrix4f& rmatWorld, BoundingVolume * pkResultBound );

		// 获取背面平面
		virtual Plane3f *    GetBackPlane( Vector3f& vPos );

	protected:
		// 球
		Sphere3f             m_kSphere;

	private:
	};
}

#endif
