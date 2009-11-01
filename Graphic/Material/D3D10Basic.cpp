#include "D3D10Basic.h"
#include "../Main/ResourceManager.h"
#include "../Main/Mesh.h"
#include "../Renderer/D3D10Effect.h"
#include "../Renderer/D3D10Texture.h"

namespace Flagship
{
	D3D10Basic::D3D10Basic()
	{
		m_mParamMap.clear();
		m_mTechniqueMap.clear();

		// 材质类型
		m_iClassType = Base::Material_General;
	}

	D3D10Basic::~D3D10Basic()
	{
		SAFE_DELETE( m_pEffect );
	}

	bool    D3D10Basic::Initialize()
	{
		// 创建D3D10Effect对象
		wchar_t szPath[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, szPath );
		wstring szEffectPath = szPath;
		szEffectPath += L"\\Effect\\D3D10Basic.fx";

		D3D10EffectHandle hEffectHandle;
		hEffectHandle.SetPath( szEffectPath );
		m_pEffect = hEffectHandle.GetImpliment();

		if ( m_szTexturePath != L"" )
		{
			D3D10TextureHandle hTextureHandle;
			hTextureHandle.SetPath( m_szTexturePath );
			m_pTexture = hTextureHandle.GetImpliment();
		}

		return true;
	}

	void    D3D10Basic::BuildParamMap()
	{
		// 建立参数表
		Key kWorld( wstring( L"g_matWorld" ) );
		Key kWorldI( wstring( L"g_matWorldI" ) );
		Key kView( wstring( L"g_matView" ) );
		Key kProj( wstring( L"g_matProj" ) );
		Key kSkinnedMap( wstring( L"g_SkinnedMap" ) );
		Key kLightDir( wstring( L"g_vLightDir" ) );
		Key kTexture( wstring( L"g_Texture" ) );

		m_mParamMap[Material::Entity_WorldMatrix] = kWorld;
		m_mParamMap[Material::Entity_WorldIMatrix] = kWorldI;
		m_mParamMap[Material::Entity_ViewMatrix] = kView;
		m_mParamMap[Material::Entity_ProjMatrix] = kProj;
		m_mParamMap[Material::AnimEntity_SkinnedMap] = kSkinnedMap;
		m_mParamMap[Material::Light_Direction] = kLightDir;
		m_mParamMap[Material::Texture_Color] = kTexture;

		m_pEffect->AddParamHandle( Effect::Param_Matrix, kWorld );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kWorldI );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kView );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kProj );
		m_pEffect->AddParamHandle( Effect::Param_Texture, kSkinnedMap );
		m_pEffect->AddParamHandle( Effect::Param_Vector, kLightDir );
		m_pEffect->AddParamHandle( Effect::Param_Texture, kTexture );

		// 建立科技表
		Key kLight( wstring( L"Light" ) );
		Key kLightS( wstring( L"LightS" ) );
		Key kColor( wstring( L"Color" ) );
		Key kColorS( wstring( L"ColorS" ) );

		m_mTechniqueMap[Material::Technique_Light] = kLight;
		m_mTechniqueMap[Material::Technique_LightS] = kLightS;
		m_mTechniqueMap[Material::Technique_Color] = kColor;
		m_mTechniqueMap[Material::Technique_ColorS] = kColorS;

		m_pEffect->AddParamHandle( Effect::Param_Technique, kLight );
		m_pEffect->AddParamHandle( Effect::Param_Technique, kLightS );
		m_pEffect->AddParamHandle( Effect::Param_Technique, kColor );
		m_pEffect->AddParamHandle( Effect::Param_Technique, kColorS );
	}
}
