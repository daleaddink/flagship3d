#include "D3D9ShadowMap.h"
#include "D3D9Renderer.h"
#include "D3D9RenderWindow.h"
#include "D3D9Texture.h"
#include "D3D9CubeTexture.h"
#include "../Main/Light.h"
#include "../Main/DataCenter.h"
#include "../Main/GlobalParam.h"

namespace Flagship
{
	D3D9ShadowMap::D3D9ShadowMap()
	{
		m_pRenderTarget = NULL;
		m_pDepthStencil = NULL;
		m_pDepthSurface = NULL;
		m_pRenderSurface = NULL;

		m_pRenderer = new D3D9Renderer;
	}

	D3D9ShadowMap::~D3D9ShadowMap()
	{
		SAFE_RELEASE( m_pRenderSurface );
		SAFE_RELEASE( m_pDepthSurface );
		SAFE_DELETE( m_pRenderTarget );
		SAFE_DELETE( m_pDepthStencil );

		SAFE_DELETE( m_pRenderer );
	}

	bool    D3D9ShadowMap::Initialize( UINT uiWidth, UINT uiHeight, DWORD dwFormat )
	{
		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 创建渲染贴图
		m_pRenderTarget = new D3D9Texture;
		if ( ! m_pRenderTarget->CreateRenderTarget( uiWidth, uiHeight, dwFormat ) )
		{
			return false;
		}

		// 创建深度贴图
		m_pDepthStencil = new D3D9Texture;
		if ( ! m_pDepthStencil->CreateDepthStencil( uiWidth, uiHeight, D3DFMT_D24S8 ) )
		{
			return false;
		}

		// 获取渲染表面
		D3D9Texture * pRenderTarget = (D3D9Texture *) m_pRenderTarget;
		pRenderTarget->GetImpliment()->GetSurfaceLevel( 0, &m_pRenderSurface );

		// 创建深度模版表面
		D3D9Texture * pDepthStencil = (D3D9Texture *) m_pDepthStencil;
		pDepthStencil->GetImpliment()->GetSurfaceLevel( 0, &m_pDepthSurface );

		m_pRenderer->SetRenderType( Renderer::RenderType_Shadow );
		return true;
	}

	void    D3D9ShadowMap::UpdateShadow( Light * pLight, BYTE byShadowFace )
	{
		ShadowMap::UpdateShadow( pLight, byShadowFace );

		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 获取旧的信息
		LPDIRECT3DSURFACE9 pOldRenderSurface;
		LPDIRECT3DSURFACE9 pOldDepthStencilSurface;
		pD3D9Device->GetRenderTarget( 0, &pOldRenderSurface );
		pD3D9Device->GetDepthStencilSurface( &pOldDepthStencilSurface );
		RenderTarget * pOldRenderTarget = GetActiveRenderTarget();
		Camera * pOldCamera = GetActiveCamera();

		// 设置渲染对象并渲染
		GlobalParam::GetSingleton()->m_pGlobalMaterial = GlobalParam::GetSingleton()->m_pMakeShadow;
		pD3D9Device->SetRenderTarget( 0, m_pRenderSurface );
		pD3D9Device->SetDepthStencilSurface( m_pDepthSurface );
		RenderTexture::Update();
		GlobalParam::GetSingleton()->m_pGlobalMaterial = NULL;

		// 恢复旧的渲染表面
		pD3D9Device->SetRenderTarget( 0, pOldRenderSurface );
		pD3D9Device->SetDepthStencilSurface( pOldDepthStencilSurface );
		SetActiveRenderTarget( pOldRenderTarget );
		SetActiveCamera( pOldCamera );

		SAFE_RELEASE( pOldRenderSurface );
		SAFE_RELEASE( pOldDepthStencilSurface );
	}

	void    D3D9ShadowMap::SaveRenderTexture( wstring szPath )
	{
		D3D9Texture * pRenderTexture = (D3D9Texture *) m_pRenderTarget;
		D3DXSaveTextureToFile( szPath.c_str(), D3DXIFF_DDS, pRenderTexture->GetImpliment(), NULL );
	}
}