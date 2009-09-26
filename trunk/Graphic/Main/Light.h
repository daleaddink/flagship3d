#ifndef _LIGHT_H_ 
#define _LIGHT_H_

#include <Common/Prerequisites.h>
#include "Entity.h"
#include "BoundingSphere.h"

namespace Flagship
{
	class _DLL_Export Light : public Entity
	{
	public:
		Light( wstring szName = L"" );
		virtual ~Light();

		enum
		{
			LightType_Directional,
			LightType_Point,
			LightType_Spot,
		};

		// ���ÿ���״̬
		void                     SetOn( bool bOn );

		// ��ȡ����״̬
		bool                     GetOn();

		// ���ù�Դ����
		void                     SetLightType( BYTE byLightType ) { m_byLightType = byLightType; }

		// ��ȡ��Դ����
		BYTE                     GetLightType() { return m_byLightType; }

		// ���ù���ɫ
		void                     SetLightColor( Vector4f& vColor );

		// ���ù�λ��
		void                     SetLightPosition( Vector4f& vPos );

		// ���ùⷽ��
		void                     SetLightDirection( Vector4f& vDir );

		// ���ù�˥������
		void                     SetLightAttenuation( Vector4f& vAttenuation );

		// �������ʵ��
		void                     AttachEntity( Entity * pEntity ) { m_pEntityList.push_back( pEntity ); }

		// ��ȡ����ʵ���б�
		list< Entity * > *       GetEntityList() { return &m_pEntityList; }

	protected:
		// ����״̬
		bool                     m_bOn;

		// ��Դ����
		BYTE                     m_byLightType;

		// ��Դ�б�
		list< Entity * >         m_pEntityList;
		
	private:

	};
}

#endif