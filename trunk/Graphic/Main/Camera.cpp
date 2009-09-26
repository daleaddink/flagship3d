#include "Camera.h"
#include "DataCenter.h"

#define m_pCameraData ( (CameraData *) m_pSharedData )

namespace Flagship
{
	Camera::Camera( wstring szName )
	{
		m_iClassType = Base::Class_Camera;
		m_byCameraType = CameraType_Perspect;
		m_pBound = NULL;

		// ÉèÖÃÃû³Æ
		if ( szName != L"" )
		{
			SetName( szName );
		}
	}

	Camera::~Camera()
	{

	}

	void    Camera::SetView( Vector4f& vPos, Vector4f& vDir, Vector4f& vUp, Vector4f& vRight )
	{
		float data[16] = { vRight[0], vUp[0], vDir[0], 0,
		                   vRight[1], vUp[1], vDir[1], 0,
		                   vRight[2], vUp[2], vDir[2], 0,
		                   - vPos.Dot( vRight ), - vPos.Dot( vUp ), - vPos.Dot( vDir ), 1 };
		Matrix4f matView( data, true );
		Matrix4f matTemp = matView.Transpose();

		( (VectorData *) m_pCameraData->m_pData[CameraData::Vector4_Position] )->Write( vPos );
		( (VectorData *) m_pCameraData->m_pData[CameraData::Vector4_Direction] )->Write( vDir );
		( (VectorData *) m_pCameraData->m_pData[CameraData::Vector4_Up] )->Write( vUp );
		( (VectorData *) m_pCameraData->m_pData[CameraData::Vector4_Right] )->Write( vRight );
		( (MatrixData *) m_pCameraData->m_pData[CameraData::Matrix4_View] )->Write( matTemp );
	}

	void    Camera::SetProjection( float fFovY, float fAspect, float fNear, float fFar )
	{
		float fScaleX, fScaleY;
		fScaleY = 1.0f / tan( fFovY / 2.0f );
		fScaleX = fScaleY / fAspect;

		float data[16] = { fScaleX, 0, 0, 0,
		                   0, fScaleY, 0, 0,
		                   0, 0, fFar / ( fFar - fNear ), 1,
		                   0, 0, - fNear * fFar / ( fFar - fNear ), 0 };
		Matrix4f matProj( data, true );
		Matrix4f matTemp = matProj.Transpose();

		m_byCameraType = CameraType_Perspect;
		( (VectorData *) m_pCameraData->m_pData[CameraData::Vector4_Param] )->Write( Vector4f( fFovY, fAspect, fNear, fFar ) );
		( (MatrixData *) m_pCameraData->m_pData[CameraData::Matrix4_Proj] )->Write( matTemp );
	}

	void    Camera::SetOrthProjection( float fWidth, float fHeight, float fNear, float fFar )
	{
		float data[16] = { 2.0f / fWidth, 0, 0, 0,
			0, 2.0f / fHeight, 0, 0,
			0, 0, 1.0f / ( fFar - fNear ), 0,
			0, 0, - fNear / ( fFar - fNear ), 1 };
		Matrix4f matProj( data, true );
		Matrix4f matTemp = matProj.Transpose();

		m_byCameraType = CameraType_Orthogon;
		( (VectorData *) m_pCameraData->m_pData[CameraData::Vector4_Param] )->Write( Vector4f( fWidth, fHeight, fNear, fFar ) );
		( (MatrixData *) m_pCameraData->m_pData[CameraData::Matrix4_Proj] )->Write( matTemp );
	}

	void    Camera::SetPosition( Vector4f& vPos )
	{
		Vector4f * pDir = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Direction )->Read();
		Vector4f * pUp = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Up )->Read();
		Vector4f * pRight = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Right )->Read();

		SetView( vPos, * pDir, * pUp, * pRight );
	}

	void    Camera::SetDirection( Vector4f& vDir )
	{
		Vector4f * pPos = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Position )->Read();
		Vector4f * pUp = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Up )->Read();
		Vector4f * pRight = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Right )->Read();

		SetView( * pPos, vDir, * pUp, * pRight );
	}

	void    Camera::SetUp( Vector4f& vUp )
	{
		Vector4f * pPos = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Position )->Read();
		Vector4f * pDir = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Direction )->Read();
		Vector4f * pRight = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Right )->Read();

		SetView( * pPos, * pDir, vUp, * pRight );
	}

	void    Camera::SetRight( Vector4f& vRight )
	{
		Vector4f * pPos = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Position )->Read();
		Vector4f * pDir = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Direction )->Read();
		Vector4f * pUp = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Up )->Read();

		SetView( * pPos, * pDir, * pUp, vRight );
	}
}