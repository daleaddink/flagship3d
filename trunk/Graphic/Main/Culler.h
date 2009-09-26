#ifndef _CULLER_H_
#define _CULLER_H_

#include <Common/Prerequisites.h>
#include "BoundingVolume.h"

namespace Flagship
{
	class Entity;
	class Portal;

	struct Segment 
	{
		Vector4f vPoint1;
		Vector4f vPoint2;

		Segment& operator = ( const Segment& rSegment )
		{
			vPoint1 = rSegment.vPoint1;
			vPoint2 = rSegment.vPoint2;

			return *this;
		}

		bool operator == ( const Segment& rSegment )
		{
			if ( ( rSegment.vPoint1 == vPoint1 && rSegment.vPoint2 == vPoint2 ) ||
			     ( rSegment.vPoint2 == vPoint1 && rSegment.vPoint1 == vPoint2 ) )
			{
				return true;
			}

			return false;
		}
	};

	struct Frustum
	{
		vector< Plane3f * >    pPlaneList;

		Frustum()
		{
			pPlaneList.clear();
		}

		~Frustum()
		{
			for ( int i = 0; i < (int) pPlaneList.size(); i++ )
			{
				SAFE_DELETE( pPlaneList[i] );
			}

			pPlaneList.clear();
		}
	};

	class _DLL_Export Culler
	{
	public:
		Culler();
		virtual ~Culler();

		// 加入剪裁面
		void                   PushPlane( Plane3f * pkPlane );

		// 踢出剪裁面
		void                   PopPlane();

		// 加入模型
		void                   PushEntity( Entity * pEntity );

		// 踢出模型
		void                   PopEntity( int iNum );

		// 加入视口
		void                   PushPortal( Portal * pPortal );

		// 踢出视口
		void                   PopPortal( int iNum );

		// 清空
		void                   Clear();

	public:
		// 包围体测试
		virtual bool           IsVisible( BoundingVolume * pBound );
		
	protected:
		// 平面列表
		list< Plane3f * >      m_pkPlaneList;

		// 场景遮挡列表
		list< Frustum * >      m_pkCollideList;

		// 场景视口列表
		list< Frustum * >      m_pkPortalList;
		
	private:
		// 边缘检测临时列表
		list< Segment >        m_pSegmentList;
	};
}

#endif