#include "D3D10MakeShadow.h"
#include "../Renderer/D3D10Effect.h"

namespace Flagship
{
	D3D10MakeShadow::D3D10MakeShadow()
	{
		m_mParamMap.clear();
		m_mTechniqueMap.clear();
		m_pEffect = NULL;
		
		// 材质类型
		m_iClassType = Base::Material_General;
	}

	D3D10MakeShadow::~D3D10MakeShadow()
	{
		SAFE_DELETE( m_pEffect );
	}

	bool    D3D10MakeShadow::Initialize()
	{
		// 创建Effect对象
		wchar_t szPath[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, szPath );
		wstring szEffectPath = szPath;
		szEffectPath += L"\\Effect\\D3D10MakeShadow.fx";

		D3D10EffectHandle hEffectHandle;
		hEffectHandle.SetPath( szEffectPath );
		m_pEffect = hEffectHandle.GetImpliment();
		m_pEffect->Load();

		return true;
	}

	void    D3D10MakeShadow::BuildParamMap()
	{
		Key kWorld( wstring( L"g_matWorld" ) );
		Key kView( wstring( L"g_matView" ) );
		Key kProj( wstring( L"g_matProj" ) );

		m_mParamMap[Material::Entity_WorldMatrix] = kWorld;
		m_mParamMap[Material::Entity_ViewMatrix] = kView;
		m_mParamMap[Material::Entity_ProjMatrix] = kProj;

		m_pEffect->AddParamHandle( Effect::Param_Matrix, kWorld );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kView );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kProj );

		// 建立科技表
		Key kDefault( wstring( L"Default" ) );
		m_mTechniqueMap[Material::Technique_MakeShadow] = kDefault;
		m_pEffect->AddParamHandle( Effect::Param_Technique, kDefault );
	}
}