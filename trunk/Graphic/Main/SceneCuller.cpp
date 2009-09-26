#include "SceneCuller.h"
#include "DataCenter.h"
#include "Camera.h"

namespace Flagship
{
	SceneCuller::SceneCuller()
	{

	}

	SceneCuller::~SceneCuller()
	{

	}

	void    SceneCuller::Initialize( Camera * pCamera )
	{
		if ( pCamera->GetCameraType() == Camera::CameraType_Perspect )
		{
			// 通过投影矩阵计算视锥的六个剪裁平面
			float fA, fB, fC, fD, fNormal;
			Matrix4f * pView = DataCenter::GetSingleton()->GetMatrixData( pCamera, pCamera->GetClassType(), CameraData::Matrix4_View )->Read();
			Matrix4f * pProj = DataCenter::GetSingleton()->GetMatrixData( pCamera, pCamera->GetClassType(), CameraData::Matrix4_Proj )->Read();
			Matrix4f matTemp = (* pProj) * (* pView);
			Matrix4f matPlane = matTemp.Transpose();

			fA = matPlane[0][3] + matPlane[0][0];
			fB = matPlane[1][3] + matPlane[1][0];
			fC = matPlane[2][3] + matPlane[2][0];
			fD = matPlane[3][3] + matPlane[3][0];
			fNormal = sqrt( SQR( fA ) + SQR( fB ) + SQR( fC ) );
			fA /= fNormal;
			fB /= fNormal;
			fC /= fNormal;
			fD /= fNormal;
			m_akViewFrustum[0].Normal[0] = fA;
			m_akViewFrustum[0].Normal[1] = fB;
			m_akViewFrustum[0].Normal[2] = fC;
			m_akViewFrustum[0].Constant = -fD;

			fA = matPlane[0][3] - matPlane[0][0];
			fB = matPlane[1][3] - matPlane[1][0];
			fC = matPlane[2][3] - matPlane[2][0];
			fD = matPlane[3][3] - matPlane[3][0];
			fNormal = sqrt( SQR( fA ) + SQR( fB ) + SQR( fC ) );
			fA /= fNormal;
			fB /= fNormal;
			fC /= fNormal;
			fD /= fNormal;
			m_akViewFrustum[1].Normal[0] = fA;
			m_akViewFrustum[1].Normal[1] = fB;
			m_akViewFrustum[1].Normal[2] = fC;
			m_akViewFrustum[1].Constant = -fD;

			fA = matPlane[0][3] + matPlane[0][1];
			fB = matPlane[1][3] + matPlane[1][1];
			fC = matPlane[2][3] + matPlane[2][1];
			fD = matPlane[3][3] + matPlane[3][1];
			fNormal = sqrt( SQR( fA ) + SQR( fB ) + SQR( fC ) );
			fA /= fNormal;
			fB /= fNormal;
			fC /= fNormal;
			fD /= fNormal;
			m_akViewFrustum[2].Normal[0] = fA;
			m_akViewFrustum[2].Normal[1] = fB;
			m_akViewFrustum[2].Normal[2] = fC;
			m_akViewFrustum[2].Constant = -fD;

			fA = matPlane[0][3] - matPlane[0][1];
			fB = matPlane[1][3] - matPlane[1][1];
			fC = matPlane[2][3] - matPlane[2][1];
			fD = matPlane[3][3] - matPlane[3][1];
			fNormal = sqrt( SQR( fA ) + SQR( fB ) + SQR( fC ) );
			fA /= fNormal;
			fB /= fNormal;
			fC /= fNormal;
			fD /= fNormal;
			m_akViewFrustum[3].Normal[0] = fA;
			m_akViewFrustum[3].Normal[1] = fB;
			m_akViewFrustum[3].Normal[2] = fC;
			m_akViewFrustum[3].Constant = -fD;

			fA = matPlane[0][2];
			fB = matPlane[1][2];
			fC = matPlane[2][2];
			fD = matPlane[3][2];
			fNormal = sqrt( SQR( fA ) + SQR( fB ) + SQR( fC ) );
			fA /= fNormal;
			fB /= fNormal;
			fC /= fNormal;
			fD /= fNormal;
			m_akViewFrustum[4].Normal[0] = fA;
			m_akViewFrustum[4].Normal[1] = fB;
			m_akViewFrustum[4].Normal[2] = fC;
			m_akViewFrustum[4].Constant = -fD;

			fA = matPlane[0][3] - matPlane[0][2];
			fB = matPlane[1][3] - matPlane[1][2];
			fC = matPlane[2][3] - matPlane[2][2];
			fD = matPlane[3][3] - matPlane[3][2];
			fNormal = sqrt( SQR( fA ) + SQR( fB ) + SQR( fC ) );
			fA /= fNormal;
			fB /= fNormal;
			fC /= fNormal;
			fD /= fNormal;
			m_akViewFrustum[5].Normal[0] = fA;
			m_akViewFrustum[5].Normal[1] = fB;
			m_akViewFrustum[5].Normal[2] = fC;
			m_akViewFrustum[5].Constant = -fD;
		}

		if ( pCamera->GetCameraType() == Camera::CameraType_Orthogon )
		{
			Vector4f * pParam = DataCenter::GetSingleton()->GetVectorData( pCamera, pCamera->GetClassType(), CameraData::Vector4_Param )->Read();
			Vector4f * pPos = DataCenter::GetSingleton()->GetVectorData( pCamera, pCamera->GetClassType(), CameraData::Vector4_Position )->Read();
			Vector4f * pDir = DataCenter::GetSingleton()->GetVectorData( pCamera, pCamera->GetClassType(), CameraData::Vector4_Direction )->Read();
			Vector4f * pUp = DataCenter::GetSingleton()->GetVectorData( pCamera, pCamera->GetClassType(), CameraData::Vector4_Up )->Read();
			Vector4f * pRight = DataCenter::GetSingleton()->GetVectorData( pCamera, pCamera->GetClassType(), CameraData::Vector4_Right )->Read();

			Vector3f vPos = Vector3f( pPos->X(), pPos->Y(), pPos->Z() );
			Vector3f vUp = Vector3f( pUp->X(), pUp->Y(), pUp->Z() );
			Vector3f vRight = Vector3f( pRight->X(), pRight->Y(), pRight->Z() );
			Vector3f vNormal = Vector3f( pDir->X(), pDir->Y(), pDir->Z() );
			vNormal.Normalize();

			m_akViewFrustum[0].Normal = vNormal;
			m_akViewFrustum[0].Constant = pParam->Z() + vPos.Dot( vNormal );
			m_akViewFrustum[1].Normal = - vNormal;
			m_akViewFrustum[1].Constant = - pParam->W() + vPos.Dot( - vNormal );
			m_akViewFrustum[2].Normal = vUp;
			m_akViewFrustum[2].Constant = - pParam->X() / 2.0f + vPos.Dot( vUp );
			m_akViewFrustum[3].Normal = - vUp;
			m_akViewFrustum[3].Constant = - pParam->X() / 2.0f + vPos.Dot( - vUp );
			m_akViewFrustum[4].Normal = vRight;
			m_akViewFrustum[4].Constant = - pParam->Y() / 2.0f + vPos.Dot( vRight );
			m_akViewFrustum[5].Normal = - vRight;
			m_akViewFrustum[5].Constant = - pParam->Y() / 2.0f + vPos.Dot( - vRight );
		}

		for ( int i = 0; i < 6; i++ )
		{
			PushPlane( &m_akViewFrustum[i] );
		}
	}
}