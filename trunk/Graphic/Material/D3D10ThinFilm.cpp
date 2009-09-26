#include "D3D10ThinFilm.h"
#include "../Main/ResourceManager.h"
#include "../Main/ResourceLoader.h"
#include "../Renderer/D3D10Texture.h"
#include "../Renderer/D3D10Effect.h"

namespace Flagship
{
	D3D10ThinFilm::D3D10ThinFilm()
	{
		m_mParamMap.clear();
		m_mTechniqueMap.clear();
		m_pFriengeTexture = NULL;

		// 材质类型
		m_iClassType = Base::Material_General;
	}

	D3D10ThinFilm::~D3D10ThinFilm()
	{
		SAFE_DELETE( m_pEffect );
		SAFE_DELETE( m_pFriengeTexture );
	}

	bool    D3D10ThinFilm::Initialize()
	{
		// 创建D3D9Effect对象
		wchar_t szPath[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, szPath );
		wstring szEffectPath = szPath;
		szEffectPath += L"\\Effect\\D3D10ThinFilm.fx";

		D3D10EffectHandle hEffectHandle;
		hEffectHandle.SetPath( szEffectPath );
		m_pEffect = hEffectHandle.GetImpliment();

		// 建立Fresnel贴图
		wstring szFriengePath = szPath;
		szFriengePath += L"\\Effect\\ThinFilmFringeMap.dds";

		D3D10TextureHandle hTextureHandle;
		hTextureHandle.SetPath( szFriengePath );
		m_pFriengeTexture = hTextureHandle.GetImpliment();

		return true;
	}

	void    D3D10ThinFilm::BuildParamMap()
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
		Key kColor( wstring( L"Color" ) );

		m_mTechniqueMap[Material::Technique_Light] = kLight;
		m_mTechniqueMap[Material::Technique_Color] = kColor;

		m_pEffect->AddParamHandle( Effect::Param_Technique, kLight );
		m_pEffect->AddParamHandle( Effect::Param_Technique, kColor );
	}

	void    D3D10ThinFilm::Update( Renderable * pParent )
	{
		D3D10Material::Update( pParent );
		m_pEffect->SetTexture( m_kFriengeTexture, m_pFriengeTexture );

		// 设置最近渲染时间
		m_pFriengeTexture->UpdateFrame();
	}

	void    D3D10ThinFilm::Update( Resource * pParent )
	{
		D3D10Material::Update( pParent );
	}

	bool    D3D10ThinFilm::IsReady()
	{
		if ( ! D3D10Material::IsReady() )
		{
			return false;
		}

		if ( ! m_pFriengeTexture->IsReady() )
		{
			return false;
		}

		return true;
	}

	bool    D3D10ThinFilm::Load()
	{
		D3D10Material::Load();
		ResourceLoader::GetSingleton()->PushResource( m_pFriengeTexture );
		return true;
	}

	bool    D3D10ThinFilm::Release()
	{
		D3D10Material::Release();
		m_pFriengeTexture->Release();
		return true;
	}
}