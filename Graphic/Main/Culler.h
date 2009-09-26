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

		// ���������
		void                   PushPlane( Plane3f * pkPlane );

		// �߳�������
		void                   PopPlane();

		// ����ģ��
		void                   PushEntity( Entity * pEntity );

		// �߳�ģ��
		void                   PopEntity( int iNum );

		// �����ӿ�
		void                   PushPortal( Portal * pPortal );

		// �߳��ӿ�
		void                   PopPortal( int iNum );

		// ���
		void                   Clear();

	public:
		// ��Χ�����
		virtual bool           IsVisible( BoundingVolume * pBound );
		
	protected:
		// ƽ���б�
		list< Plane3f * >      m_pkPlaneList;

		// �����ڵ��б�
		list< Frustum * >      m_pkCollideList;

		// �����ӿ��б�
		list< Frustum * >      m_pkPortalList;
		
	private:
		// ��Ե�����ʱ�б�
		list< Segment >        m_pSegmentList;
	};
}

#endif