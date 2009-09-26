#include "FreeCamera.h"
#include "../../Graphic/Main/DataCenter.h"

namespace Flagship
{
	FreeCamera::FreeCamera( wstring szName )
		: Camera( szName )
	{
		
	}

	FreeCamera::~FreeCamera()
	{

	}

	void    FreeCamera::MoveForward( float fDist )
	{
		Vector4f * pPos = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Position )->Read();
		Vector4f * pDir = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Direction )->Read();

		Vector4f kNewPos = (* pPos) + (* pDir) * fDist;
		SetPosition( kNewPos );
	}

	void    FreeCamera::MoveBackward( float fDist )
	{
		Vector4f * pPos = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Position )->Read();
		Vector4f * pDir = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Direction )->Read();

		Vector4f kNewPos = (* pPos) - (* pDir) * fDist;
		SetPosition( kNewPos );
	}
	
	void    FreeCamera::MoveLeft( float fDist )
	{
		Vector4f * pPos = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Position )->Read();
		Vector4f * pRight = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Right )->Read();

		Vector4f kNewPos = (* pPos) - (* pRight) * fDist;
		SetPosition( kNewPos );
	}

	void    FreeCamera::MoveRight( float fDist )
	{
		Vector4f * pPos = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Position )->Read();
		Vector4f * pRight = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Right )->Read();

		Vector4f kNewPos = (* pPos) + (* pRight) * fDist;
		SetPosition( kNewPos );
	}

	void    FreeCamera::MoveUp( float fDist )
	{
		Vector4f * pPos = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Position )->Read();
		Vector4f * pUp = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Up )->Read();

		Vector4f kNewPos = (* pPos) + (* pUp) * fDist;
		SetPosition( kNewPos );
	}

	void    FreeCamera::MoveDown( float fDist )
	{
		Vector4f * pPos = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Position )->Read();
		Vector4f * pUp = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Up )->Read();

		Vector4f kNewPos = (* pPos) - (* pUp) * fDist;
		SetPosition( kNewPos );
	}

	void    FreeCamera::Rotate( float fYaw, float fPitch, float fRoll )
	{
		Vector4f * pPos = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Position )->Read();
		Vector4f * pDir = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Direction )->Read();
		Vector4f * pUp = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Up )->Read();
		Vector4f * pRight = DataCenter::GetSingleton()->GetVectorData( this, m_iClassType, CameraData::Vector4_Right )->Read();

		Vector3f kNewDir( (* pDir)[0], (* pDir)[1], (* pDir)[2] );
		Vector3f kNewUp( (* pUp)[0], (* pUp)[1], (* pUp)[2] );
		Vector3f kNewRight( (* pRight)[0], (* pRight)[1], (* pRight)[2] );

		Matrix3f kRotateMatrix( Matrix3f::IDENTITY );
		Matrix3f kTempMatrix;
		kTempMatrix.FromAxisAngle( kNewUp, fYaw );
		kRotateMatrix = kRotateMatrix * kTempMatrix;
		kTempMatrix.FromAxisAngle( kNewRight, fPitch );
		kRotateMatrix = kRotateMatrix * kTempMatrix;
		kTempMatrix.FromAxisAngle( kNewDir, fRoll );
		kRotateMatrix = kRotateMatrix * kTempMatrix;

		kNewDir = kRotateMatrix * kNewDir;
		kNewUp = kRotateMatrix * kNewUp;
		kNewRight = kRotateMatrix * kNewRight;
		
		SetView( (* pPos)
			, Vector4f( kNewDir[0], kNewDir[1], kNewDir[2], 0.0f )
			, Vector4f( kNewUp[0], kNewUp[1], kNewUp[2], 0.0f )
			, Vector4f( kNewRight[0], kNewRight[1], kNewRight[2], 0.0f ) );
	}
}