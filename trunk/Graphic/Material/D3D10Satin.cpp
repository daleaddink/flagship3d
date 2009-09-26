#include "D3D10Satin.h"
#include "../Main/ResourceManager.h"
#include "../Main/ResourceLoader.h"
#include "../Renderer/D3D10VolumeTexture.h"
#include "../Renderer/D3D10Effect.h"

namespace Flagship
{
	D3D10Satin::D3D10Satin()
	{
		m_mParamMap.clear();
		m_mTechniqueMap.clear();

		// 材质类型
		m_iClassType = Base::Material_General;
	}

	D3D10Satin::~D3D10Satin()
	{
		SAFE_DELETE( m_pEffect );
		SAFE_DELETE( m_pNoiseTexture );
	}

	bool    D3D10Satin::Initialize()
	{
		// 创建D3D9Effect对象
		wchar_t szPath[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, szPath );
		wstring szEffectPath = szPath;
		szEffectPath += L"\\Effect\\D3D10Satin.fx";

		D3D10EffectHandle hEffectHandle;
		hEffectHandle.SetPath( szEffectPath );
		m_pEffect = hEffectHandle.GetImpliment();

		// 建立Noise贴图
		wstring szNoisePath = szPath;
		szNoisePath += L"\\Effect\\SatinNoise.dds";

		D3D10VolumeTextureHandle hTextureHandle;
		hTextureHandle.SetPath( szNoisePath );
		m_pNoiseTexture = hTextureHandle.GetImpliment();

		return true;
	}

	void    D3D10Satin::BuildParamMap()
	{
		// 建立参数表
		Key kWorld( wstring( L"g_matWorld" ) );
		Key kView( wstring( L"g_matView" ) );
		Key kProj( wstring( L"g_matProj" ) );
		Key kSkinnedMatrix( wstring( L"g_matSkinnedMatrix" ) );
		Key kLightDir( wstring( L"g_vLightDir" ) );
		m_kNoiseTexture.SetName( wstring(L"g_NoiseTexture") );

		m_mParamMap[Material::Entity_WorldMatrix] = kWorld;
		m_mParamMap[Material::Entity_ViewMatrix] = kView;
		m_mParamMap[Material::Entity_ProjMatrix] = kProj;
		m_mParamMap[Material::AnimEntity_SkinnedMatrix] = kSkinnedMatrix;
		m_mParamMap[Material::Light_Direction] = kLightDir;

		m_pEffect->AddParamHandle( Effect::Param_Matrix, kWorld );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kView );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kProj );
		m_pEffect->AddParamHandle( Effect::Param_MatrixArray, kSkinnedMatrix );
		m_pEffect->AddParamHandle( Effect::Param_Vector, kLightDir );
		m_pEffect->AddParamHandle( Effect::Param_Texture, m_kNoiseTexture );

		// 建立科技表
		Key kLight( wstring( L"Light" ) );
		Key kColor( wstring( L"Color" ) );

		m_mTechniqueMap[Material::Technique_Light] = kLight;
		m_mTechniqueMap[Material::Technique_Color] = kColor;

		m_pEffect->AddParamHandle( Effect::Param_Technique, kLight );
		m_pEffect->AddParamHandle( Effect::Param_Technique, kColor );
	}

	void    D3D10Satin::Update( Renderable * pParent )
	{
		Material::Update( pParent );
		m_pEffect->SetTexture( m_kNoiseTexture, m_pNoiseTexture );

		// 设置最近渲染时间
		m_pNoiseTexture->UpdateFrame();
	}

	void    D3D10Satin::Update( Resource * pParent )
	{
		Material::Update( pParent );
	}

	bool    D3D10Satin::IsReady()
	{
		if ( ! Material::IsReady() )
		{
			return false;
		}

		if ( ! m_pNoiseTexture->IsReady() )
		{
			return false;
		}

		return true;
	}

	bool    D3D10Satin::Load()
	{
		Material::Load();
		ResourceLoader::GetSingleton()->PushResource( m_pNoiseTexture );
		return true;
	}

	bool    D3D10Satin::Release()
	{
		Material::Release();
		m_pNoiseTexture->Release();
		return true;
	}
}