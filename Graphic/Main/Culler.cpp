#include "Culler.h"
#include "Entity.h"
#include "Mesh.h"
#include "Portal.h"
#include "Camera.h"
#include "DataCenter.h"
#include "RenderTarget.h"

namespace Flagship
{
	Culler::Culler()
	{
		m_pkPlaneList.clear();
		m_pSegmentList.clear();
		m_pkCollideList.clear();
		m_pkPortalList.clear();
	}

	Culler::~Culler()
	{
		Clear();
	}

	void    Culler::PushPlane( Plane3f * pkPlane )
	{
		m_pkPlaneList.push_back( pkPlane );
	}

	void    Culler::PopPlane()
	{
		SAFE_DELETE( m_pkPlaneList.back() );
		m_pkPlaneList.pop_back();
	}

	void    Culler::PushEntity( Entity * pEntity )
	{
		Segment kSegment[3];
		Vector4f kVector[3];
		Vector3f kPoint[2];

		// 获取当前摄像机
		Camera * pCamera = RenderTarget::GetActiveCamera();
		Vector4f * pEyeDir = DataCenter::GetSingleton()->GetVectorData( pCamera, pCamera->GetClassType(), CameraData::Vector4_Direction )->Read();
		Vector4f * pEyePos = DataCenter::GetSingleton()->GetVectorData( pCamera, pCamera->GetClassType(), CameraData::Vector4_Position )->Read();
		Vector3f vDir = Vector3f( pEyeDir->X(), pEyeDir->Y(), pEyeDir->Z() );
		Vector3f vPos = Vector3f( pEyePos->X(), pEyePos->Y(), pEyePos->Z() );

		Matrix4f * pWorld = DataCenter::GetSingleton()->GetMatrixData( pEntity, pEntity->GetClassType(), RenderableData::Matrix4_World )->Read();
		Matrix4f matWorldIT = * pWorld;
		matWorldIT = matWorldIT.Inverse();
		matWorldIT = matWorldIT.Transpose();

		// 边缘检测
		Mesh * pCullMesh = pEntity->GetInnerBound();
		if ( pCullMesh != NULL )
		{
			Vector3f * pVertex = pCullMesh->GetVertexData();
			WORD * pIndex = pCullMesh->GetIndexData();
			Vector3f * pNormal = pCullMesh->GetNormalData();

			for ( int i = 0; i < (int) pCullMesh->GetNumTriangle(); i++ )
			{
				kVector[0] = Vector4f( pNormal[pIndex[i * 3]][0], pNormal[pIndex[i * 3]][1], pNormal[pIndex[i * 3]][2], 0.0f );
				kVector[1] = Vector4f( pNormal[pIndex[i * 3 + 1]][0], pNormal[pIndex[i * 3 + 1]][1], pNormal[pIndex[i * 3 + 1]][2], 0.0f );
				kVector[2] = Vector4f( pNormal[pIndex[i * 3 + 2]][0], pNormal[pIndex[i * 3 + 2]][1], pNormal[pIndex[i * 3 + 2]][2], 0.0f );

				Vector4f vFaceNormal = ( matWorldIT * kVector[0] + 
					                     matWorldIT * kVector[1] + 
										 matWorldIT * kVector[2] ) / 3;
				if ( vDir.Dot( Vector3f( vFaceNormal.X(), vFaceNormal.Y(), vFaceNormal.Z() ) ) >= 0 )
				{
					continue;
				}

				kVector[0] = Vector4f( pVertex[pIndex[i * 3]][0], pVertex[pIndex[i * 3]][1], pVertex[pIndex[i * 3]][2], 1.0f );
				kVector[1] = Vector4f( pVertex[pIndex[i * 3 + 1]][0], pVertex[pIndex[i * 3 + 1]][1], pVertex[pIndex[i * 3 + 1]][2], 1.0f );
				kVector[2] = Vector4f( pVertex[pIndex[i * 3 + 2]][0], pVertex[pIndex[i * 3 + 2]][1], pVertex[pIndex[i * 3 + 2]][2], 1.0f );

				kSegment[0].vPoint1 = (*pWorld) * kVector[0];
				kSegment[0].vPoint2 = (*pWorld) * kVector[1];
				kSegment[1].vPoint1 = (*pWorld) * kVector[1];
				kSegment[1].vPoint2 = (*pWorld) * kVector[2];
				kSegment[2].vPoint1 = (*pWorld) * kVector[2];
				kSegment[2].vPoint2 = (*pWorld) * kVector[0];

				for ( int j = 0; j < 3; j++ )
				{
					bool bExist = false;;
					list< Segment >::iterator SegIt = m_pSegmentList.begin();
					while ( SegIt != m_pSegmentList.end() )
					{
						if ( (*SegIt) == kSegment[j] )
						{
							m_pSegmentList.erase( SegIt );
							bExist = true;
							break;
						}

						SegIt++;
					}

					if ( !bExist )
					{
						m_pSegmentList.push_back( kSegment[j] );
					}
				}
			}

			// 添加遮挡体
			Frustum * pCollide = new Frustum;
			list< Segment >::iterator RstIt = m_pSegmentList.begin();
			while ( RstIt != m_pSegmentList.end() )
			{
				kPoint[0] = Vector3f( (*RstIt).vPoint1.X(), (*RstIt).vPoint1.Y(), (*RstIt).vPoint1.Z() );
				kPoint[1] = Vector3f( (*RstIt).vPoint2.X(), (*RstIt).vPoint2.Y(), (*RstIt).vPoint2.Z() );

				Plane3f * pPlane = new Plane3f( kPoint[0], kPoint[1], vPos );
				pCollide->pPlaneList.push_back( pPlane );

				RstIt++;
			}

			if ( pCollide->pPlaneList.size() != 0 )
			{
				// 添加背面平面
				BoundingVolume * pVolume = pEntity->GetBound();
				pCollide->pPlaneList.push_back( pVolume->GetBackPlane( vPos ) );
				m_pkCollideList.push_back( pCollide );
			}

			m_pSegmentList.clear();
		}
	}

	void    Culler::PopEntity( int iNum )
	{
		for ( int i = 0; i < iNum; i++ )
		{
			SAFE_DELETE( m_pkCollideList.back() );
			m_pkCollideList.pop_back();
		}
	}

	void    Culler::PushPortal( Portal * pPortal )
	{

	}

	void    Culler::PopPortal( int iNum )
	{
		for ( int i = 0; i < iNum; i++ )
		{
			SAFE_DELETE( m_pkPortalList.back() );
			m_pkCollideList.pop_back();
		}
	}

	void    Culler::Clear()
	{
		m_pkPlaneList.clear();
		m_pkCollideList.clear();
		m_pkPortalList.clear();
	}

	bool    Culler::IsVisible( BoundingVolume * pBound )
	{
		if ( pBound == NULL )
		{
			return true;
		}

		list< Plane3f * >::iterator PlaneIt;
		for ( PlaneIt = m_pkPlaneList.begin(); PlaneIt != m_pkPlaneList.end(); ++PlaneIt )
		{
			if ( pBound->IsCulled( (*PlaneIt) ) )
			{
				return false;
			}
		}

		list< Frustum * >::iterator PortalIt;
		for ( PortalIt = m_pkPortalList.begin(); PortalIt != m_pkPortalList.end(); ++PortalIt )
		{
			vector< Plane3f * >::iterator DetalIt;
			for ( DetalIt = (*PortalIt)->pPlaneList.begin(); DetalIt != (*PortalIt)->pPlaneList.end(); ++DetalIt )
			{
				if ( pBound->IsCulled( (*DetalIt) ) )
				{
					return false;
				}
			}
		}

		list< Frustum * >::iterator CollideIt;
		for ( CollideIt = m_pkCollideList.begin(); CollideIt != m_pkCollideList.end(); ++CollideIt )
		{
			bool bCulled = true;
			vector< Plane3f * >::iterator DetalIt;
			for ( DetalIt = (*CollideIt)->pPlaneList.begin(); DetalIt != (*CollideIt)->pPlaneList.end(); ++DetalIt )
			{
				if ( ! pBound->IsCulled( (*DetalIt) ) )
				{
					bCulled = false;
					break;
				}
			}

			if ( bCulled )
			{
				return false;
			}
		}

		return true;
	}
}