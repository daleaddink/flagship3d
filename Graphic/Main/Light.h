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

		// 设置开关状态
		void                     SetOn( bool bOn );

		// 获取开关状态
		bool                     GetOn();

		// 设置光源类型
		void                     SetLightType( BYTE byLightType ) { m_byLightType = byLightType; }

		// 获取光源类型
		BYTE                     GetLightType() { return m_byLightType; }

		// 设置光颜色
		void                     SetLightColor( Vector4f& vColor );

		// 设置光位置
		void                     SetLightPosition( Vector4f& vPos );

		// 设置光方向
		void                     SetLightDirection( Vector4f& vDir );

		// 设置光衰减函数
		void                     SetLightAttenuation( Vector4f& vAttenuation );

		// 添加作用实体
		void                     AttachEntity( Entity * pEntity ) { m_pEntityList.push_back( pEntity ); }

		// 获取作用实体列表
		list< Entity * > *       GetEntityList() { return &m_pEntityList; }

	protected:
		// 开关状态
		bool                     m_bOn;

		// 光源类型
		BYTE                     m_byLightType;

		// 资源列表
		list< Entity * >         m_pEntityList;
		
	private:

	};
}

#endif