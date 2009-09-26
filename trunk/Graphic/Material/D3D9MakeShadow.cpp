#include "D3D9MakeShadow.h"
#include "../Renderer/D3D9Effect.h"
#include "../Main/Texture.h"

namespace Flagship
{
	D3D9MakeShadow::D3D9MakeShadow()
	{
		m_mParamMap.clear();
		m_mTechniqueMap.clear();
		m_pEffect = NULL;

		// 材质类型
		m_iClassType = Base::Material_General;
	}

	D3D9MakeShadow::~D3D9MakeShadow()
	{
		SAFE_DELETE( m_pEffect );
	}

	bool    D3D9MakeShadow::Initialize()
	{
		// 创建Effect对象
		wchar_t szPath[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, szPath );
		wstring szEffectPath = szPath;
		szEffectPath += L"\\Effect\\D3D9MakeShadow.fx";

		D3D9EffectHandle hEffectHandle;
		hEffectHandle.SetPath( szEffectPath );
		m_pEffect = hEffectHandle.GetImpliment();
		m_pEffect->Load();
		m_pSkinnedMap->CreateDynamic( 1024, 1, D3DFMT_A32B32G32R32F );

		return true;
	}

	void    D3D9MakeShadow::BuildParamMap()
	{
		Key kWorld( wstring( L"g_matWorld" ) );
		Key kView( wstring( L"g_matView" ) );
		Key kProj( wstring( L"g_matProj" ) );
		Key kSkinnedMap( wstring( L"g_SkinnedMap" ) );

		m_mParamMap[Material::Entity_WorldMatrix] = kWorld;
		m_mParamMap[Material::Entity_ViewMatrix] = kView;
		m_mParamMap[Material::Entity_ProjMatrix] = kProj;
		m_mParamMap[Material::AnimEntity_SkinnedMap] = kSkinnedMap;

		m_pEffect->AddParamHandle( Effect::Param_Matrix, kWorld );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kView );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kProj );
		m_pEffect->AddParamHandle( Effect::Param_Texture, kSkinnedMap );

		// 建立科技表
		Key kMakeShadow( wstring( L"MakeShadow" ) );
		Key kMakeShadowS( wstring( L"MakeShadowS" ) );
		m_mTechniqueMap[Material::Technique_MakeShadow] = kMakeShadow;
		m_mTechniqueMap[Material::Technique_MakeShadowS] = kMakeShadowS;
		m_pEffect->AddParamHandle( Effect::Param_Technique, kMakeShadow );
		m_pEffect->AddParamHandle( Effect::Param_Technique, kMakeShadowS );
	}
}