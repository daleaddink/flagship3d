#include "Light.h"
#include "DataCenter.h"
#include "BoundingSphere.h"

#define m_pLightData ( (LightData *) m_pSharedData )

namespace Flagship
{
	Light::Light( wstring szName )
	{
		m_iClassType = Entity_Light;
		m_dwLastFrame = 0;
		m_bCastShadow = false;
		m_bAffectByLight = false;

		// 设置名称
		if ( szName != L"" )
		{
			SetName( szName );
		}

		// 设置包围体
		m_pBound = new BoundingSphere;
		m_pLocalBound = new BoundingSphere;
		* m_pLocalBound = * ( (BoundingSphere *) m_pBound );
	}

	Light::~Light()
	{

	}

	void    Light::SetOn( bool bOn )
	{
		m_bOn = bOn;
	}

	bool    Light::GetOn()
	{
		return m_bOn;
	}

	void    Light::SetLightColor( Vector4f& vColor )
	{
		( (VectorData *) m_pLightData->m_pData[LightData::Vector4_Color] )->Write( vColor );
	}

	void    Light::SetLightPosition( Vector4f& vPos )
	{
		( (VectorData *) m_pLightData->m_pData[LightData::Vector4_Position] )->Write( vPos );

		// 与实体位置同步
		Matrix4f * pWorld = DataCenter::GetSingleton()->GetMatrixData( this, m_iClassType, LightData::Matrix4_World )->Read();
		Matrix4f matWorld = * pWorld;
		matWorld[3][0] = vPos[0];
		matWorld[3][1] = vPos[1];
		matWorld[3][2] = vPos[2];
		Entity::SetWorldMatrix( matWorld );
	}

	void    Light::SetLightDirection( Vector4f& vDir )
	{
		( (VectorData *) m_pLightData->m_pData[LightData::Vector4_Direction] )->Write( vDir );
	}

	void    Light::SetLightAttenuation( Vector4f& vAttenuation )
	{
		( (VectorData *) m_pLightData->m_pData[LightData::Vector4_Attenuation] )->Write( vAttenuation );
	}
}