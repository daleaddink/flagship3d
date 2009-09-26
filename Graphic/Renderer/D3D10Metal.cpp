#include "D3D10Metal.h"
#include "../Main/DataCenter.h"
#include "../Main/ResourceManager.h"
#include "../Renderer/D3D10RenderWindow.h"
#include "../Renderer/D3D10Renderer.h"
#include "../Renderer/D3D10Effect.h"
#include "../Renderer/D3D10RenderCubeTexture.h"

namespace Flagship
{
	D3D10Metal::D3D10Metal()
	{
		m_mParamMap.clear();
		m_mTechniqueMap.clear();

		// 材质类型
		m_iClassType = Base::Material_RenderTexture;
	}

	D3D10Metal::~D3D10Metal()
	{
		SAFE_DELETE( m_pEffect );
		SAFE_DELETE( m_pRenderCamera );
		SAFE_DELETE( m_pCubeTextureRenderer );
		SAFE_DELETE( m_pRenderCubeTexture );
	}

	bool    D3D10Metal::Initialize()
	{
		// 创建D3D9Effect对象
		wchar_t szPath[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, szPath );
		wstring szEffectPath = szPath;
		szEffectPath += L"\\Effect\\D3D10Metal.fx";

		D3D10EffectHandle hEffectHandle;
		hEffectHandle.SetPath( szEffectPath );
		m_pEffect = hEffectHandle.GetImpliment();

		// 创建渲染贴图摄像机
		m_pRenderCamera = new Camera( L"MetalCamera" );
		m_pRenderCamera->SetProjection( 3.14f / 2.0f, 1.0f, 1.0f, 100.0f );

		// 建立渲染贴图
		m_pCubeTextureRenderer = new D3D10Renderer;
		m_pRenderCubeTexture = new D3D10RenderCubeTexture;

		// 获取后备缓冲格式
		ID3D10RenderTargetView * pRenderTarget = ( (D3D10RenderWindow *) RenderWindow::GetActiveRenderWindow() )->GetRenderTargetView();
		D3D10_RENDER_TARGET_VIEW_DESC kRenderTargetDesc;
		pRenderTarget->GetDesc( &kRenderTargetDesc );

		m_pRenderCubeTexture->Initialize( 256, kRenderTargetDesc.Format );
		m_pRenderCubeTexture->SetCamera( m_pRenderCamera );
		m_pRenderCubeTexture->SetRenderer( m_pCubeTextureRenderer );

		return true;
	}

	void    D3D10Metal::BuildParamMap()
	{
		// 建立参数表
		Key kWorld( wstring( L"g_matWorld" ) );
		Key kView( wstring( L"g_matView" ) );
		Key kProj( wstring( L"g_matProj" ) );
		Key kWorldI( wstring( L"g_matWorldI" ) );
		Key kViewI( wstring( L"g_matViewI" ) );
		Key kSkinnedMatrix( wstring( L"g_matSkinnedMatrix" ) );
		Key kLightDir( wstring( L"g_vLightDir" ) );
		m_kRenderCubeTexture.SetName( wstring( L"g_CubeTexture" ) );

		m_mParamMap[Material::Entity_WorldMatrix] = kWorld;
		m_mParamMap[Material::Entity_ViewMatrix] = kView;
		m_mParamMap[Material::Entity_ProjMatrix] = kProj;
		m_mParamMap[Material::Entity_WorldIMatrix] = kWorldI;
		m_mParamMap[Material::Entity_ViewIMatrix] = kViewI;
		m_mParamMap[Material::AnimEntity_SkinnedMatrix] = kSkinnedMatrix;
		m_mParamMap[Material::DirctionalLight_Direction] = kLightDir;

		m_pEffect->AddParamHandle( Effect::Param_Matrix, kWorld );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kView );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kProj );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kWorldI );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kViewI );
		m_pEffect->AddParamHandle( Effect::Param_MatrixArray, kSkinnedMatrix );
		m_pEffect->AddParamHandle( Effect::Param_Vector, kLightDir );
		m_pEffect->AddParamHandle( Effect::Param_Texture, m_kRenderCubeTexture );

		// 建立科技表
		Key kNormal( wstring( L"Normal" ) );
		Key kSkinned( wstring( L"Skinned" ) );

		m_mTechniqueMap[Material::Technique_Normal] = kNormal;
		m_mTechniqueMap[Material::Technique_Skinned] = kSkinned;

		m_pEffect->AddParamHandle( Effect::Param_Technique, kNormal );
		m_pEffect->AddParamHandle( Effect::Param_Technique, kSkinned );
	}

	void    D3D10Metal::Update( Renderable * pParent )
	{
		Material::Update( pParent );

		// 更新渲染贴图
		Matrix4f * pWorld = DataCenter::GetSingleton()->GetMatrix( pParent, pParent->GetClassType(), RenderableData::Matrix4_World );
		Matrix4f matWorld = * pWorld;
		Vector4f vPos = matWorld.GetColumn( 3 );
		m_pRenderCamera->SetPosition( vPos );

		m_pRenderCubeTexture->Update();
		m_pEffect->SetTexture( m_kRenderCubeTexture, m_pRenderCubeTexture->GetRenderTarget() );
	}

	void    D3D10Metal::Update( Resource * pParent )
	{

	}
}
