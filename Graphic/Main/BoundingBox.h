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

		// 重载等于
		BoundingBox&         operator = ( BoundingBox& rkBound );

		// 设置中心
		void                 SetCenter( Vector3f& rvCenter ) { m_kBox.Center = rvCenter; }

		// 设置坐标
		void                 SetAxis( Vector3f * avAxis );

		// 设置扩展
		void                 SetExtent( float * afExtent );

		// 获取盒子
		Box3f&               GetBox() { return m_kBox; }

	public:
		// 扩大包容
		virtual void         GrowToContain( BoundingVolume * pBound );

		// 判断位置
		virtual bool         IsInBound( Vector3f& vPos );

		// 是否被剪裁
		virtual bool         IsCulled( Plane3f * pkPlane );

		// 是否相交
		virtual bool         IsIntersect( BoundingVolume * pBound );

		// 坐标变换
		virtual void         TransformBy( Matrix4f& rmatWorld, BoundingVolume * pkResultBound );

		// 获取背面平面
		virtual Plane3f *    GetBackPlane( Vector3f& vPos );

	protected:
		// 盒子
		Box3f           m_kBox;

	private:
	};
}

#endif
