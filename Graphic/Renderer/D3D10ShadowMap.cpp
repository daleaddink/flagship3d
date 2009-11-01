#include "D3D10ShadowMap.h"
#include "D3D10Renderer.h"
#include "D3D10RenderWindow.h"
#include "D3D10Texture.h"
#include "D3D10CubeTexture.h"
#include "../Main/Light.h"
#include "../Main/DataCenter.h"
#include "../Main/GlobalParam.h"

namespace Flagship
{
	D3D10ShadowMap::D3D10ShadowMap()
	{
		m_pRenderTarget = NULL;
		m_pDepthStencil = NULL;
		m_pDepthSurface = NULL;
		m_pRenderSurface = NULL;

		m_pRenderer = new D3D10Renderer;
	}

	D3D10ShadowMap::~D3D10ShadowMap()
	{
		SAFE_RELEASE( m_pRenderSurface );
		SAFE_RELEASE( m_pDepthSurface );
		SAFE_DELETE( m_pRenderTarget );
		SAFE_DELETE( m_pDepthStencil );

		SAFE_DELETE( m_pRenderer );
	}

	bool    D3D10ShadowMap::Initialize( UINT uiWidth, UINT uiHeight, DWORD dwFormat )
	{
		// 获取D3D10设备指针
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 创建渲染贴图
		m_pRenderTarget = new D3D10Texture;
		if ( ! m_pRenderTarget->CreateRenderTarget( uiWidth, uiHeight, dwFormat ) )
		{
			return false;
		}

		// 获取渲染表面
		D3D10Texture * pRenderTarget = (D3D10Texture *) m_pRenderTarget;
		m_pRenderSurface = pRenderTarget->GetRenderTargetView();

		// 创建渲染贴图
		m_pDepthStencil = new D3D10Texture;
		if ( ! m_pDepthStencil->CreateDepthStencil( uiWidth, uiHeight, DXGI_FORMAT_D24_UNORM_S8_UINT  ) )
		{
			return false;
		}

		// 获取深度表面
		D3D10Texture * pDepthStencil = (D3D10Texture *) m_pDepthStencil;
		m_pDepthSurface = pDepthStencil->GetDepthStencilView();

		return true;
	}

	void    D3D10ShadowMap::UpdateShadow( Light * pLight, BYTE byShadowFace )
	{
		ShadowMap::UpdateShadow( pLight, byShadowFace );

		// 获取D3D10设备指针
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 获取旧的信息
		ID3D10RenderTargetView * pOldRenderSurface;
		ID3D10DepthStencilView * pOldDepthSurface;
		pD3D10Device->OMGetRenderTargets( 1, &pOldRenderSurface, &pOldDepthSurface );
		RenderTarget * pOldRenderTarget = GetActiveRenderTarget();
		Camera * pOldCamera = GetActiveCamera();

		// 设置渲染对象并渲染
		GlobalParam::GetSingleton()->m_pGlobalMaterial = GlobalParam::GetSingleton()->m_pMakeShadow;
		pD3D10Device->OMSetRenderTargets( 1, &m_pRenderSurface, m_pDepthSurface );
		RenderTexture::Update();
		GlobalParam::GetSingleton()->m_pGlobalMaterial = NULL;

		// 恢复旧的渲染表面
		pD3D10Device->OMSetRenderTargets( 1, &pOldRenderSurface, pOldDepthSurface );
		SetActiveRenderTarget( pOldRenderTarget );
		SetActiveCamera( pOldCamera );

		SAFE_RELEASE( pOldRenderSurface );
		SAFE_RELEASE( pOldDepthSurface );
	}

	void    D3D10ShadowMap::SaveRenderTexture( wstring szPath )
	{
		D3D10Texture * pRenderTarget = (D3D10Texture *) m_pRenderTarget;
		D3DX10SaveTextureToFile( pRenderTarget->GetImpliment(), D3DX10_IFF_DDS, szPath.c_str() );
	}
}