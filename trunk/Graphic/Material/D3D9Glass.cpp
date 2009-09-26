#include "D3D9Glass.h"
#include "../Main/DataCenter.h"
#include "../Main/ResourceManager.h"
#include "../Main/ResourceLoader.h"
#include "../Renderer/D3D9RenderWindow.h"
#include "../Renderer/D3D9Texture.h"
#include "../Renderer/D3D9XMesh.h"
#include "../Renderer/D3D9Renderer.h"
#include "../Renderer/D3D9Effect.h"
#include "../Renderer/D3D9RenderCubeTexture.h"

namespace Flagship
{
	D3D9Glass::D3D9Glass()
	{
		m_mParamMap.clear();
		m_mTechniqueMap.clear();

		// ��������
		m_iClassType = Base::Material_RenderTexture;
	}

	D3D9Glass::~D3D9Glass()
	{
		SAFE_DELETE( m_pEffect );
		SAFE_DELETE( m_pFresnelTexture );
		SAFE_DELETE( m_pRenderCubeTexture );
	}

	bool    D3D9Glass::Initialize()
	{
		// ����Effect����
		wchar_t szPath[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, szPath );
		wstring szEffectPath = szPath;
		szEffectPath += L"\\Effect\\D3D9Glass.fx";

		D3D9EffectHandle hEffectHandle;
		hEffectHandle.SetPath( szEffectPath );
		m_pEffect = hEffectHandle.GetImpliment();
		
		// ����Fresnel��ͼ
		wstring szFresnelPath = szPath;
		szFresnelPath += L"\\Effect\\GlassFresnel.dds";

		D3D9TextureHandle hTextureHandle;
		hTextureHandle.SetPath( szFresnelPath );
		m_pFresnelTexture = hTextureHandle.GetImpliment();
		
		// ������Ⱦ��ͼ
		m_pRenderCubeTexture = new D3D9RenderCubeTexture;

		// ��ȡ�󱸻����ʽ
		LPDIRECT3DSURFACE9 pRenderTarget = ( (D3D9RenderWindow *) RenderWindow::GetActiveRenderWindow() )->GetRenderTarget();
		D3DSURFACE_DESC kRenderTargetDesc;
		pRenderTarget->GetDesc( &kRenderTargetDesc );

		m_pRenderCubeTexture->Initialize( 256, kRenderTargetDesc.Format );
		m_pRenderCubeTexture->GetCamera()->SetProjection( 3.14f / 2.0f, 1.0f, 0.1f, 500.0f );
		m_pRenderCubeTexture->GetRenderer()->SetRenderType( Renderer::RenderType_NoReflect );

		return true;
	}

	void    D3D9Glass::BuildParamMap()
	{
		// ����������
		Key kWorld( wstring( L"g_matWorld" ) );
		Key kView( wstring( L"g_matView" ) );
		Key kProj( wstring( L"g_matProj" ) );
		Key kWorldI( wstring( L"g_matWorldI" ) );
		Key kViewI( wstring( L"g_matViewI" ) );
		Key kSkinnedMatrix( wstring( L"g_matSkinnedMatrix" ) );
		m_kFresnelTexture.SetName( wstring( L"g_FresnelTexture" ) );
		m_kRenderCubeTexture.SetName( wstring( L"g_CubeTexture" ) );

		m_mParamMap[Material::Entity_WorldMatrix] = kWorld;
		m_mParamMap[Material::Entity_ViewMatrix] = kView;
		m_mParamMap[Material::Entity_ProjMatrix] = kProj;
		m_mParamMap[Material::Entity_WorldIMatrix] = kWorldI;
		m_mParamMap[Material::Entity_ViewIMatrix] = kViewI;
		m_mParamMap[Material::AnimEntity_SkinnedMatrix] = kSkinnedMatrix;

		m_pEffect->AddParamHandle( Effect::Param_Matrix, kWorld );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kView );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kProj );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kWorldI );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kViewI );
		m_pEffect->AddParamHandle( Effect::Param_MatrixArray, kSkinnedMatrix );
		m_pEffect->AddParamHandle( Effect::Param_Texture, m_kFresnelTexture );
		m_pEffect->AddParamHandle( Effect::Param_Texture, m_kRenderCubeTexture );

		// �����Ƽ���
		Key kLight( wstring( L"Light" ) );
		Key kColor( wstring( L"Color" ) );

		m_mTechniqueMap[Material::Technique_Light] = kLight;
		m_mTechniqueMap[Material::Technique_Color] = kColor;

		m_pEffect->AddParamHandle( Effect::Param_Technique, kLight );
		m_pEffect->AddParamHandle( Effect::Param_Technique, kColor );
	}

	void    D3D9Glass::Update( Renderable * pParent )
	{
		Material::Update( pParent );

		// ������Ⱦ��ͼ
		Matrix4f * pWorld = DataCenter::GetSingleton()->GetMatrixData( pParent, pParent->GetClassType(), RenderableData::Matrix4_World )->Get();
		Matrix4f matWorld = * pWorld;
		Vector4f vPos = matWorld.GetColumn( 3 );
		m_pRenderCubeTexture->GetCamera()->SetPosition( vPos );

		m_pRenderCubeTexture->Update();
		m_pEffect->SetTexture( m_kFresnelTexture, m_pFresnelTexture );
		m_pEffect->SetTexture( m_kRenderCubeTexture, m_pRenderCubeTexture->GetRenderTarget() );
	}

	void    D3D9Glass::Update( Resource * pParent )
	{
		Material::Update( pParent );
	}

	bool    D3D9Glass::IsReady()
	{
		if ( ! Material::IsReady() )
		{
			return false;
		}

		if ( ! m_pFresnelTexture->IsReady() )
		{
			return false;
		}

		return true;
	}

	bool    D3D9Glass::Load()
	{
		Material::Load();
		ResourceLoader::GetSingleton()->PushResource( m_pFresnelTexture );
		return true;
	}

	bool    D3D9Glass::Release()
	{
		Material::Release();
		m_pFresnelTexture->Release();
		return true;
	}
}