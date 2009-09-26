#include "D3D9ThinFilm.h"
#include "../Main/ResourceManager.h"
#include "../Main/ResourceLoader.h"
#include "../Renderer/D3D9Texture.h"
#include "../Renderer/D3D9Effect.h"

namespace Flagship
{
	D3D9ThinFlim::D3D9ThinFlim()
	{
		m_mParamMap.clear();
		m_mTechniqueMap.clear();
		m_pFriengeTexture = NULL;

		// 材质类型
		m_iClassType = Base::Material_General;
	}

	D3D9ThinFlim::~D3D9ThinFlim()
	{
		SAFE_DELETE( m_pEffect );
		SAFE_DELETE( m_pFriengeTexture );
	}

	bool    D3D9ThinFlim::Initialize()
	{
		// 创建D3D9Effect对象
		wchar_t szPath[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, szPath );
		wstring szEffectPath = szPath;
		szEffectPath += L"\\Effect\\D3D9ThinFilm.fx";

		D3D9EffectHandle hEffectHandle;
		hEffectHandle.SetPath( szEffectPath );
		m_pEffect = hEffectHandle.GetImpliment();

		// 建立Fresnel贴图
		wstring szFriengePath = szPath;
		szFriengePath += L"\\Effect\\ThinFilmFringeMap.dds";

		D3D9TextureHandle hTextureHandle;
		hTextureHandle.SetPath( szFriengePath );
		m_pFriengeTexture = hTextureHandle.GetImpliment();

		return true;
	}

	void    D3D9ThinFlim::BuildParamMap()
	{
		// 建立参数表
		Key kWorld( wstring( L"g_matWorld" ) );
		Key kView( wstring( L"g_matView" ) );
		Key kProj( wstring( L"g_matProj" ) );
		Key kWorldI( wstring( L"g_matWorldI" ) );
		Key kViewI( wstring( L"g_matViewI" ) );
		Key kSkinnedMap( wstring( L"g_SkinnedMap" ) );
		Key kLightDir( wstring( L"g_vLightDir" ) );
		m_kFriengeTexture.SetName( wstring( L"g_FringeMap" ) );

		m_mParamMap[Material::Entity_WorldMatrix] = kWorld;
		m_mParamMap[Material::Entity_ViewMatrix] = kView;
		m_mParamMap[Material::Entity_ProjMatrix] = kProj;
		m_mParamMap[Material::Entity_WorldIMatrix] = kWorldI;
		m_mParamMap[Material::Entity_ViewIMatrix] = kViewI;
		m_mParamMap[Material::AnimEntity_SkinnedMap] = kSkinnedMap;
		m_mParamMap[Material::Light_Direction] = kLightDir;
		
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kWorld );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kView );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kProj );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kWorldI );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kViewI );
		m_pEffect->AddParamHandle( Effect::Param_Texture, kSkinnedMap );
		m_pEffect->AddParamHandle( Effect::Param_Vector, kLightDir );
		m_pEffect->AddParamHandle( Effect::Param_Texture, m_kFriengeTexture );
		
		// 建立科技表
		Key kLight( wstring( L"Light" ) );
		Key kLightSkinned( wstring( L"LightSkinned" ) );
		m_mTechniqueMap[Material::Technique_Light] = kLight;
		m_mTechniqueMap[Material::Technique_LightS] = kLightSkinned;
		m_pEffect->AddParamHandle( Effect::Param_Technique, kLight );
		m_pEffect->AddParamHandle( Effect::Param_Technique, kLightSkinned );
	}

	void    D3D9ThinFlim::Update( Renderable * pParent )
	{
		D3D9Material::Update( pParent );
		m_pEffect->SetTexture( m_kFriengeTexture, m_pFriengeTexture );

		// 设置最近渲染时间
		m_pFriengeTexture->UpdateFrame();
	}

	void    D3D9ThinFlim::Update( Resource * pParent )
	{
		D3D9Material::Update( pParent );
	}

	bool    D3D9ThinFlim::IsReady()
	{
		if ( ! D3D9Material::IsReady() )
		{
			return false;
		}

		if ( ! m_pFriengeTexture->IsReady() )
		{
			return false;
		}

		return true;
	}

	bool    D3D9ThinFlim::Load()
	{
		D3D9Material::Load();
		ResourceLoader::GetSingleton()->PushResource( m_pFriengeTexture );
		return true;
	}

	bool    D3D9ThinFlim::Release()
	{
		D3D9Material::Release();
		m_pFriengeTexture->Release();
		return true;
	}
}