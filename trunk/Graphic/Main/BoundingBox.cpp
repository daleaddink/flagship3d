#include "BoundingBox.h"
#include "BoundingSphere.h"

namespace Flagship
{
	BoundingBox::BoundingBox()
		: m_kBox( Vector3f::ZERO, Vector3f::UNIT_X, Vector3f::UNIT_Y, Vector3f::UNIT_Z,	0.1f, 0.1f, 0.1f )
	{
		m_iBoundType = BoundingVolume::Bounding_Box;
	}

	BoundingBox::BoundingBox( Box3f& rkBox )
	{
		m_iBoundType = BoundingVolume::Bounding_Box;
		m_kBox = rkBox;
	}

	BoundingBox::BoundingBox( Vector3f& vCenter, Vector3f * avAxis, float * afExtent)
	{
		m_iBoundType = BoundingVolume::Bounding_Box;

		SetCenter( vCenter );

		if ( avAxis == NULL )
		{
			Vector3f avTemp[3] = { Vector3f::UNIT_X, Vector3f::UNIT_Y, Vector3f::UNIT_Z };
			SetAxis( avTemp );
		}
		else
		{
    		SetAxis( avAxis );
		}

		if ( afExtent == NULL )
		{
			float afTemp[3] = { 1.0f, 1.0f, 1.0f };
			SetExtent( afTemp );
		}
		else
		{
    		SetExtent( afExtent );
		}
	}

	BoundingBox::~BoundingBox()
	{

	}

	BoundingBox&    BoundingBox::operator = ( BoundingBox& rkBound )
	{
		SetCenter( rkBound.GetBox().Center );
		SetAxis( rkBound.GetBox().Axis );
		SetExtent( rkBound.GetBox().Extent );

		return * this;
	}

	void    BoundingBox::SetAxis( Vector3f * avAxis )
	{
		for ( int i = 0; i < 3; i++ )
		{
			m_kBox.Axis[i] = avAxis[i];
		}
	}

	void    BoundingBox::SetExtent( float * afExtent )
	{
		for ( int i = 0; i < 3; i++ )
		{
			m_kBox.Extent[i] = afExtent[i];
		}
	}

	void    BoundingBox::GrowToContain( BoundingVolume * pBound )
	{
		switch ( pBound->GetBoundType() )
		{
		case Bounding_Box:
			{
				m_kBox = MergeBoxes( m_kBox, ( (BoundingBox *) pBound )->GetBox() );
				break;
			}
		case Bounding_Sphere:
			{
				Sphere3f& rkSphere = ( (BoundingSphere *) pBound )->GetSphere();
				BoundingBox kTempBox;
				kTempBox.SetCenter( rkSphere.Center );
				float fExtent[3] = { rkSphere.Radius, rkSphere.Radius, rkSphere.Radius };
				kTempBox.SetExtent( fExtent );

				m_kBox = MergeBoxes( m_kBox, kTempBox.GetBox() );
				break;
			}
		default:
			break;
		}
	}

	bool    BoundingBox::IsInBound( Vector3f& vPos )
	{
		return InBox( vPos, m_kBox );
	}

	bool    BoundingBox::IsCulled( Plane3f * pkPlane )
	{
		float fProjCenter = pkPlane->Normal.Dot( m_kBox.Center ) - pkPlane->Constant;
		float fAbs0 = Mathf::FAbs( pkPlane->Normal.Dot( m_kBox.Axis[0] ) );
		float fAbs1 = Mathf::FAbs( pkPlane->Normal.Dot(m_kBox.Axis[1] ) );
		float fAbs2 = Mathf::FAbs( pkPlane->Normal.Dot(m_kBox.Axis[2] ) );
		float fProjRadius = m_kBox.Extent[0] * fAbs0 + m_kBox.Extent[1] * fAbs1 + m_kBox.Extent[2] * fAbs2;

		if ( fProjCenter + fProjRadius <= 0.0f )
		{
			return true;
		}

		return false;
	}

	bool    BoundingBox::IsIntersect( BoundingVolume * pBound )
	{
		switch ( pBound->GetBoundType() )
		{
		case Bounding_Box:
			{
				IntrBox3Box3f kIntr( ( (BoundingBox *) pBound )->GetBox(), GetBox() );
				return kIntr.Test();
				break;
			}
		case Bounding_Sphere:
			{
				IntrBox3Sphere3f kIntr( GetBox(), ( (BoundingSphere *) pBound )->GetSphere() );
				return kIntr.Test();
				break;
			}
		default:
			break;
		}

		return false;
	}

	void    BoundingBox::TransformBy( Matrix4f& rmatWorld, BoundingVolume * pkResultBound )
	{
		Box3f& rkBox = ( (BoundingBox *) pkResultBound )->GetBox();
		
		Vector4f vSrc( m_kBox.Center[0], m_kBox.Center[1], m_kBox.Center[2], 1.0f );
		Vector4f vDst = rmatWorld * vSrc;
		rkBox.Center = Vector3f( vDst[0], vDst[1], vDst[2] );

		for ( int i = 0; i < 3; i++ )
		{
			vSrc = Vector4f( m_kBox.Axis[i][0], m_kBox.Axis[i][1], m_kBox.Axis[i][2], 0.0f );
			vDst = rmatWorld * vSrc;

			rkBox.Axis[i] = Vector3f( vDst[0], vDst[1], vDst[2] );
			rkBox.Extent[i] = m_kBox.Extent[i] * rkBox.Axis[i].Length();
			rkBox.Axis[i].Normalize();
		}
	}

	Plane3f *    BoundingBox::GetBackPlane( Vector3f& vPos )
	{
		Vector3f vNormal = vPos - m_kBox.Center;
		vNormal.Normalize();
		Vector3f vPoint = m_kBox.Center - vNormal * sqrt( SQR( m_kBox.Extent[0] ) + SQR( m_kBox.Extent[1] ) + SQR( m_kBox.Extent[2] ) );

		Plane3f * pPlane = new Plane3f( vNormal, vPoint );
		return pPlane;
	}
}