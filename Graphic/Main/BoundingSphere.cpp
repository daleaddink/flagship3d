#include "BoundingSphere.h"
#include "BoundingBox.h"

namespace Flagship
{
	BoundingSphere::BoundingSphere()
		: m_kSphere( Vector3f::ZERO, 1.0f )
	{
		m_iBoundType = BoundingVolume::Bounding_Sphere;
	}

	BoundingSphere::BoundingSphere( Sphere3f& rkSphere )
	{
		m_iBoundType = BoundingVolume::Bounding_Sphere;
		m_kSphere = rkSphere;
	}

	BoundingSphere::BoundingSphere( Vector3f& vCenter, float fRadius )
	{
		m_iBoundType = BoundingVolume::Bounding_Sphere;

		SetCenter( vCenter );

		if ( fRadius == 0 )
		{
			SetRadius( 1.0f );
		}
		else
		{
			SetRadius( fRadius );
		}
	}

	BoundingSphere::~BoundingSphere()
	{

	}

	BoundingSphere&    BoundingSphere::operator = ( BoundingSphere& rkBound )
	{
		SetCenter( rkBound.GetSphere().Center );
		SetRadius( rkBound.GetSphere().Radius );

		return * this;
	}

	void    BoundingSphere::GrowToContain( BoundingVolume * pBound )
	{
		switch ( pBound->GetBoundType() )
		{
		case Bounding_Box:
			{
				Vector3f akVector[8];
				( (BoundingBox *) pBound )->GetBox().ComputeVertices( akVector );
				m_kSphere = MergeSpheres( m_kSphere, ContSphereOfAABB( 8, akVector ) );
				break;
			}
		case Bounding_Sphere:
			{
				 m_kSphere = MergeSpheres( m_kSphere,( (BoundingSphere *) pBound )->GetSphere() );
				break;
			}
		default:
			break;
		}
	}

	bool    BoundingSphere::IsInBound( Vector3f& rvPos )
	{
		return InSphere( rvPos, m_kSphere );
	}

	bool    BoundingSphere::IsCulled( Plane3f * pkPlane )
	{
		float fDistance = pkPlane->DistanceTo( m_kSphere.Center );

		if ( fDistance <= -m_kSphere.Radius )
		{
			return true;
		}

		return false;
	}

	bool    BoundingSphere::IsIntersect( BoundingVolume * pBound )
	{
		switch ( pBound->GetBoundType() )
		{
		case Bounding_Box:
			{
				IntrBox3Sphere3f kIntr( ( (BoundingBox *) pBound )->GetBox(), GetSphere() );
				return kIntr.Test();
				break;
			}
		case Bounding_Sphere:
			{
				IntrSphere3Sphere3f kIntr( ( (BoundingSphere *) pBound )->GetSphere(), GetSphere() );
				return kIntr.Test();
				break;
			}
		default:
			break;
		}

		return false;
	}

	void    BoundingSphere::TransformBy( Matrix4f& rmatWorld, BoundingVolume * pkResultBound )
	{
		Sphere3f& rkSphere = ( (BoundingSphere *) pkResultBound )->GetSphere();

		Vector4f vSrc( m_kSphere.Center[0], m_kSphere.Center[1], m_kSphere.Center[2], 1.0f );
		Vector4f vDst = rmatWorld * vSrc;
		rkSphere.Center = Vector3f( vDst[0], vDst[1], vDst[2] );
		rkSphere.Radius = m_kSphere.Radius;
	}

	Plane3f *    BoundingSphere::GetBackPlane( Vector3f& vPos )
	{
		Vector3f vNormal = vPos - m_kSphere.Center;
		vNormal.Normalize();
		Vector3f vPoint = m_kSphere.Center - vNormal * m_kSphere.Radius;

		Plane3f * pPlane = new Plane3f( vNormal, vPoint );
		return pPlane;
	}
}
