#include "D3D9RenderWindow.h"
#include "D3D9Renderer.h"
#include "D3D9RenderTexture.h"
#include "D3D9RenderCubeTexture.h"
#include "D3D9ShadowMap.h"
#include "../Material/D3D9MakeShadow.h"
#include "../Material/D3D9PostShadow.h"
#include "../Material/D3D9Ambient.h"
#include "../Main/GlobalParam.h"

namespace Flagship
{
	D3D9RenderWindow::D3D9RenderWindow()
	{
		m_pD3D = NULL;
		m_pD3D9Device = NULL;
		m_pLDRSurfaceRT = NULL;
		m_pLDRSurfaceDS = NULL;
		m_pHDRSurfaceRT = NULL;
		m_pHDRSurfaceDS = NULL;

		m_pRenderer = new D3D9Renderer;
		for ( int i = 0; i < MAX_LIGHT * 4; i++ )
		{
			m_pShadowMap[i] = new D3D9ShadowMap;
		}
	}

	D3D9RenderWindow::~D3D9RenderWindow()
	{
		SAFE_RELEASE( m_pD3D );
		SAFE_RELEASE( m_pD3D9Device );
		
		SAFE_RELEASE( m_pLDRSurfaceRT );
		SAFE_RELEASE( m_pLDRSurfaceDS );
		SAFE_RELEASE( m_pHDRSurfaceRT );
		SAFE_RELEASE( m_pHDRSurfaceDS );

		SAFE_DELETE( m_pRenderer );
		SAFE_DELETE( GlobalParam::GetSingleton()->m_pMakeShadow );
		SAFE_DELETE( GlobalParam::GetSingleton()->m_pPostShadow );
		SAFE_DELETE( GlobalParam::GetSingleton()->m_pAmbient );

		for ( int i = 0; i < MAX_LIGHT; i++ )
		{
			SAFE_DELETE( m_pShadowMap[i] );
		}
	}

	bool    D3D9RenderWindow::Initialize( bool bFullScreen, bool bHDR )
	{
		// 初始化D3D
		m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

		if ( m_pD3D == NULL )
		{
			char szLog[10240];
			sprintf( szLog, "D3D9RenderWindow::Initialize():Direct3DCreate9 Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		// 初始化设备
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory( &d3dpp, sizeof(d3dpp) );
		d3dpp.Windowed = ! bFullScreen;
		d3dpp.hDeviceWindow = m_hWindowHandle;
		d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    	HRESULT hr;
		hr = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWindowHandle, D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED, &d3dpp, &m_pD3D9Device );

		if( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D9RenderWindow::Initialize():CreateDevice() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		// 创建字体
		D3DXCreateFont( m_pD3D9Device, 24, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
		                OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
		                L"Arial", &m_pFont );

		// LDR渲染对象
		m_pD3D9Device->GetRenderTarget( 0, &m_pLDRSurfaceRT );
		m_pD3D9Device->GetDepthStencilSurface( &m_pLDRSurfaceDS );

		// HDR渲染对象
		m_bHDR = bHDR;
		if ( bHDR )
		{
			// 获取后备缓冲格式
			LPDIRECT3DSURFACE9 pBackBuffer;
			m_pD3D9Device->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
			D3DSURFACE_DESC kBackBufferDesc;
			pBackBuffer->GetDesc( &kBackBufferDesc );
			SAFE_RELEASE( pBackBuffer );

			m_pD3D9Device->CreateRenderTarget( kBackBufferDesc.Width, kBackBufferDesc.Height,
				                               D3DFMT_A16B16G16R16F, D3DMULTISAMPLE_NONE, 0,
				                               false, &m_pHDRSurfaceRT, NULL );
			m_pD3D9Device->CreateDepthStencilSurface( kBackBufferDesc.Width, kBackBufferDesc.Height,
				                                      D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0,
				                                      true, &m_pHDRSurfaceDS, NULL );
			m_pD3D9Device->SetRenderTarget( 0, m_pHDRSurfaceRT );
			m_pD3D9Device->SetDepthStencilSurface( m_pHDRSurfaceDS );
		}

		RenderWindow::SetActiveRenderWindow( this );

        GlobalParam::GetSingleton()->m_pMakeShadow = new D3D9MakeShadow;
		GlobalParam::GetSingleton()->m_pMakeShadow->Initialize();

		GlobalParam::GetSingleton()->m_pPostShadow = new D3D9PostShadow;
		GlobalParam::GetSingleton()->m_pPostShadow->Initialize();

		GlobalParam::GetSingleton()->m_pAmbient = new D3D9Ambient;
		GlobalParam::GetSingleton()->m_pAmbient->Initialize();

		for ( int i = 0; i < MAX_LIGHT * 4; i++ )
		{
			m_pShadowMap[i]->Initialize( 1024, 1024, D3DFMT_R32F );
		}

		return true;
	}

	void    D3D9RenderWindow::RestoreRenderTarget()
	{
		m_pD3D9Device->SetRenderTarget( 0, m_pLDRSurfaceRT );
		m_pD3D9Device->SetDepthStencilSurface( m_pLDRSurfaceDS );
	}

	LPDIRECT3DSURFACE9    D3D9RenderWindow::GetRenderTarget()
	{
		if ( m_bHDR )
		{
			return m_pHDRSurfaceRT;
		}

		return m_pLDRSurfaceRT;
	}

	LPDIRECT3DSURFACE9    D3D9RenderWindow::GetDepthStencil()
	{
		if ( m_bHDR )
		{
			return m_pHDRSurfaceDS;
		}
		
		return m_pLDRSurfaceDS;
	}

	void    D3D9RenderWindow::Update()
	{
		if ( m_bHDR )
		{
			m_pD3D9Device->SetRenderTarget( 0, m_pHDRSurfaceRT );
			m_pD3D9Device->SetDepthStencilSurface( m_pHDRSurfaceDS );
		}

		RenderWindow::Update();
	}

	void    D3D9RenderWindow::SaveRenderTexture( wstring szPath )
	{
		LPDIRECT3DSURFACE9 pRenderSurface;
		m_pD3D9Device->GetRenderTarget( 0, &pRenderSurface );
		D3DXSaveSurfaceToFile( szPath.c_str(), D3DXIFF_DDS, pRenderSurface, NULL, NULL );
	}

	LPDIRECT3DDEVICE9    D3D9RenderWindow::GetDevice()
	{
		return m_pD3D9Device;
	}

	LPD3DXFONT    D3D9RenderWindow::GetFont()
	{
		return m_pFont;
	}
}