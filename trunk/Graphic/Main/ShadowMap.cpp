#include "ShadowMap.h"
#include "Light.h"
#include "Material.h"
#include "DataCenter.h"
#include "Renderer.h"

namespace Flagship
{
	ShadowMap::ShadowMap()
	{
		
	}

	ShadowMap::~ShadowMap()
	{

	}

	Matrix4f *    ShadowMap::GetLightMatrix()
	{
		Matrix4f * pLightView = DataCenter::GetSingleton()->GetMatrixData( m_pCamera
			, m_pCamera->GetClassType(), CameraData::Matrix4_View )->Get();
		Matrix4f * pLightProj = DataCenter::GetSingleton()->GetMatrixData( m_pCamera
			, m_pCamera->GetClassType(), CameraData::Matrix4_Proj )->Get();
		m_matLight = (*pLightProj) * (*pLightView);

		return &m_matLight;
	}

	bool    ShadowMap::Initialize( UINT uiWidth, UINT uiHeight, DWORD dwFormat )
	{
		return true;
	}

	void    ShadowMap::UpdateShadow( Light * pLight, BYTE byShadowFace )
	{
		Matrix4f * pWorld = DataCenter::GetSingleton()->GetMatrixData( pLight, pLight->GetClassType(), RenderableData::Matrix4_World )->Get();
		Matrix4f matWorld = * pWorld;
		Vector4f vPos = matWorld.GetColumn( 3 );

		m_pRenderer->Clear( Vector4f( 1.0f, 1.0f, 1.0f, 1.0f ) );

		switch ( pLight->GetLightType() )
		{
		case Light::LightType_Directional:
			{
				Vector4f * pDir = DataCenter::GetSingleton()->GetVectorData( pLight, pLight->GetClassType()
					, LightData::Vector4_Direction )->Get();

				Vector3f vDir( pDir->X(), pDir->Y(), pDir->Z() );
				Vector3f vUp( 0, 1, 0 );
				Vector3f vRight = vUp.Cross( vDir );
				vUp = vDir.Cross( vRight );
				m_pCamera->SetView( Vector4f( 0, 0, 0, 0 ), (*pDir), Vector4f( vUp.X(), vUp.Y(), vUp.Z(), 0 ), Vector4f( vRight.X(), vRight.Y(), vRight.Z(), 0 ) );

				BoundingBox kShadowBox;
				BoundingBox kTransBox;
				BoundingSphere kTransSphere;

				Renderer * pRenderer = RenderTarget::GetActiveRenderTarget()->GetRenderer();
				multimap< float, Entity * > * pVisibleList = pRenderer->GetRenderQueue( Renderer::RenderQueue_Visible )->GetEntityList();

				bool bFirst = true;
				multimap< float, Entity * >::iterator it;

				Matrix4f * pView = DataCenter::GetSingleton()->GetMatrixData( m_pCamera, m_pCamera->GetClassType()
					, CameraData::Matrix4_View )->Read();

				for ( it = pVisibleList->begin(); it != pVisibleList->end(); it++ )
				{
					if ( (*it).second->IsCastShadow() && (*it).second->GetBound() != NULL )
					{
						if ( (*it).second->GetBound()->GetBoundType() == BoundingVolume::Bounding_Box )
						{
						    (*it).second->GetBound()->TransformBy( (*pView), &kTransBox );

							if ( bFirst )
							{
								kShadowBox.SetCenter( kTransBox.GetBox().Center );
								bFirst = false;
							}

						    kShadowBox.GrowToContain( &kTransBox );
						}

						if ( (*it).second->GetBound()->GetBoundType() == BoundingVolume::Bounding_Sphere )
						{
							(*it).second->GetBound()->TransformBy( (*pView), &kTransSphere );

							if ( bFirst )
							{
								kShadowBox.SetCenter( kTransSphere.GetSphere().Center );
								bFirst = false;
							}

							kShadowBox.GrowToContain( &kTransSphere );
						}
					}
				}

				Matrix4f matViewI = pView->Inverse();
				Vector4f vViewCenter = Vector4f( kShadowBox.GetBox().Center.X(), kShadowBox.GetBox().Center.Y(), kShadowBox.GetBox().Center.Z(), 1.0f );
				Vector4f vWorldCenter = matViewI * vViewCenter;

				Vector3f kNewPos = Vector3f( vWorldCenter.X(), vWorldCenter.Y(), vWorldCenter.Z() );
				m_pCamera->SetView( Vector4f( kNewPos.X(), kNewPos.Y(), kNewPos.Z(), 1.0f ), (*pDir)
					, Vector4f( vUp.X(), vUp.Y(), vUp.Z(), 0 ), Vector4f( vRight.X(), vRight.Y(), vRight.Z(), 0 ) );
				float fLength = sqrt( SQR( kShadowBox.GetBox().Extent[0] ) + SQR( kShadowBox.GetBox().Extent[1] ) + SQR( kShadowBox.GetBox().Extent[2] ) );
				m_pCamera->SetOrthProjection( fLength * 2.0f, fLength * 2.0f, - fLength, fLength * 2.0f );

				break;
			}
		case Light::LightType_Point:
			{
				Vector4f vDir, vRight, vUp;
				if ( byShadowFace == Shadow_PositiveX )
				{
					vDir = Vector4f( 1.0f, 0.0f, 0.0f, 0.0f );
					vRight = Vector4f( 0.0f, 0.0f, -1.0f, 0.0f );
					vUp = Vector4f( 0.0f, 1.0f, 0.0f, 0.0f );
					m_pCamera->SetView( vPos, vDir, vUp, vRight );
				}

				if ( byShadowFace == Shadow_NegativeX )
				{
					vDir = Vector4f( -1.0f, 0.0f, 0.0f, 0.0f );
					vRight = Vector4f( 0.0f, 0.0f, 1.0f, 0.0f );
					vUp = Vector4f( 0.0f, 1.0f, 0.0f, 0.0f );
					m_pCamera->SetView( vPos, vDir, vUp, vRight );
				}

				if ( byShadowFace == Shadow_PositiveY )
				{
					vDir = Vector4f( 0.0f, 1.0f, 0.0f, 0.0f );
					vRight = Vector4f( 1.0f, 0.0f, 0.0f, 0.0f );
					vUp = Vector4f( 0.0f, 0.0f, -1.0f, 0.0f );
					m_pCamera->SetView( vPos, vDir, vUp, vRight );
				}

				if ( byShadowFace == Shadow_NegativeY )
				{
					vDir = Vector4f( 0.0f, -1.0f, 0.0f, 0.0f );
					vRight = Vector4f( 1.0f, 0.0f, 0.0f, 0.0f );
					vUp = Vector4f( 0.0f, 0.0f, 1.0f, 0.0f );
					m_pCamera->SetView( vPos, vDir, vUp, vRight );
				}

				if ( byShadowFace == Shadow_PositiveZ )
				{
					vDir = Vector4f( 0.0f, 0.0f, 1.0f, 0.0f );
					vRight = Vector4f( 1.0f, 0.0f, 0.0f, 0.0f );
					vUp = Vector4f( 0.0f, 1.0f, 0.0f, 0.0f );
					m_pCamera->SetView( vPos, vDir, vUp, vRight );
				}

				if ( byShadowFace == Shadow_NegativeZ )
				{
					vDir = Vector4f( 0.0f, 0.0f, -1.0f, 0.0f );
					vRight = Vector4f( -1.0f, 0.0f, 0.0f, 0.0f );
					vUp = Vector4f( 0.0f, 1.0f, 0.0f, 0.0f );
					m_pCamera->SetView( vPos, vDir, vUp, vRight );
				}

				m_pCamera->SetProjection( 3.14f / 2.0f, 1.0f, 0.1f, 100.0f );
				break;
			}
		case Light::LightType_Spot:
			{
				Vector4f * pDir = DataCenter::GetSingleton()->GetVectorData( pLight, pLight->GetClassType(), LightData::Vector4_Direction )->Get();
				float * pPhi = DataCenter::GetSingleton()->GetFloatData( pLight, pLight->GetClassType(), LightData::Float_Phi )->Get();

				Vector3f vDir( pDir->X(), pDir->Y(), pDir->Z() );
				Vector3f vUp( 0, 1, 0 );
				Vector3f vRight = vDir.Cross( vUp );
				vUp = vRight.Cross( vDir );

				m_pCamera->SetView( vPos, (*pDir), Vector4f( vUp.X(), vUp.Y(), vUp.Z(), 1 )
					, Vector4f( vRight.X(), vRight.Y(), vRight.Z(), 1 ) );
				m_pCamera->SetProjection( (*pPhi) * 2.0f, 1.0f, 0.1f, 100.0f );
				break;
			}
		}
	}
}