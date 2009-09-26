#include "D3D9Ambient.h"
#include "../Renderer/D3D9Effect.h"
#include "../Main/Texture.h"
#include "../Main/DataCenter.h"
#include "../Main/GlobalParam.h"

namespace Flagship
{
	D3D9Ambient::D3D9Ambient()
	{
		m_mParamMap.clear();
		m_mTechniqueMap.clear();

		// 材质类型
		m_iClassType = Base::Material_General;
	}

	D3D9Ambient::~D3D9Ambient()
	{
		SAFE_DELETE( m_pEffect );
	}

	bool    D3D9Ambient::Initialize()
	{
		// 创建D3D9Effect对象
		wchar_t szPath[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, szPath );
		wstring szEffectPath = szPath;
		szEffectPath += L"\\Effect\\D3D9Ambient.fx";

		D3D9EffectHandle hEffectHandle;
		hEffectHandle.SetPath( szEffectPath );
		m_pEffect = hEffectHandle.GetImpliment();
		m_pEffect->Load();
		m_pSkinnedMap->CreateDynamic( 1024, 1, D3DFMT_A32B32G32R32F );

		return true;
	}

	void    D3D9Ambient::BuildParamMap()
	{
		// 建立参数表
		Key kWorld( wstring( L"g_matWorld" ) );
		Key kView( wstring( L"g_matView" ) );
		Key kProj( wstring( L"g_matProj" ) );
		Key kColor( wstring( L"g_vAmbientColor" ) );
		Key kSkinnedMap( wstring( L"g_SkinnedMap" ) );

		m_mParamMap[Material::Entity_WorldMatrix] = kWorld;
		m_mParamMap[Material::Entity_ViewMatrix] = kView;
		m_mParamMap[Material::Entity_ProjMatrix] = kProj;
        m_mParamMap[Material::Light_Ambient] = kColor;
		m_mParamMap[Material::AnimEntity_SkinnedMap] = kSkinnedMap;
		
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kWorld );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kView );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kProj );
		m_pEffect->AddParamHandle( Effect::Param_Vector, kColor );
		m_pEffect->AddParamHandle( Effect::Param_Texture, kSkinnedMap );

		// 建立科技表
		Key kAmbient( wstring( L"Ambient" ) );
		Key kAmbientS( wstring( L"AmbientS" ) );
		m_mTechniqueMap[Material::Technique_Ambient] = kAmbient;
		m_mTechniqueMap[Material::Technique_AmbientS] = kAmbientS;
		m_pEffect->AddParamHandle( Effect::Param_Technique, kAmbient );
		m_pEffect->AddParamHandle( Effect::Param_Technique, kAmbientS );
	}
}
