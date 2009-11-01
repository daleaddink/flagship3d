#include "D3D10PostShadow.h"
#include "../Renderer/D3D10Effect.h"
#include "../Main/GlobalParam.h"
#include "../Main/Texture.h"
#include "../Main/ShadowMap.h"

namespace Flagship
{
	D3D10PostShadow::D3D10PostShadow()
	{
		m_mParamMap.clear();
		m_mTechniqueMap.clear();
		m_pEffect = NULL;

		// 材质类型
		m_iClassType = Base::Material_General;
	}

	D3D10PostShadow::~D3D10PostShadow()
	{
		SAFE_DELETE( m_pEffect );
	}

	bool    D3D10PostShadow::Initialize()
	{
		// 创建Effect对象
		wchar_t szPath[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, szPath );
		wstring szEffectPath = szPath;
		szEffectPath += L"\\Effect\\D3D10PostShadow.fx";

		D3D10EffectHandle hEffectHandle;
		hEffectHandle.SetPath( szEffectPath );
		m_pEffect = hEffectHandle.GetImpliment();
		m_pEffect->Load();
		m_pSkinnedMap->CreateDynamic( 1024, 1, D3DFMT_A32B32G32R32F );

		return true;
	}

	void    D3D10PostShadow::BuildParamMap()
	{
		Key kWorld( wstring( L"g_matWorld" ) );
		Key kView( wstring( L"g_matView" ) );
		Key kProj( wstring( L"g_matProj" ) );
		Key kSkinnedMap( wstring( L"g_SkinnedMap" ) );
		m_kShadowMap.SetName( wstring( L"g_ShadowMap" ) );
		m_kLightMatrix.SetName( wstring( L"g_LightMatrix" ) );

		m_mParamMap[Material::Entity_WorldMatrix] = kWorld;
		m_mParamMap[Material::Entity_ViewMatrix] = kView;
		m_mParamMap[Material::Entity_ProjMatrix] = kProj;
		m_mParamMap[Material::AnimEntity_SkinnedMap] = kSkinnedMap;

		m_pEffect->AddParamHandle( Effect::Param_Matrix, kWorld );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kView );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kProj );
		m_pEffect->AddParamHandle( Effect::Param_Texture, kSkinnedMap );
		m_pEffect->AddParamHandle( Effect::Param_Texture, m_kShadowMap );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, m_kLightMatrix );

		// 建立科技表
		Key kPostShadow( wstring( L"PostShadow" ) );
		Key kPostShadowS( wstring( L"PostShadowS" ) );
		m_mTechniqueMap[Material::Technique_PostShadow] = kPostShadow;
		m_mTechniqueMap[Material::Technique_PostShadowS] = kPostShadowS;
		m_pEffect->AddParamHandle( Effect::Param_Technique, kPostShadow );
		m_pEffect->AddParamHandle( Effect::Param_Technique, kPostShadowS );
	}

	void    D3D10PostShadow::Update( Renderable * pParent )
	{
		D3D10Material::Update( pParent );

		// 更新阴影图和光源矩阵
		m_pEffect->SetTexture( m_kShadowMap, GlobalParam::GetSingleton()->m_pCurShadowMap->GetRenderTarget() );
		m_pEffect->SetMatrix( m_kLightMatrix, GlobalParam::GetSingleton()->m_pCurShadowMap->GetLightMatrix() );
	}
}