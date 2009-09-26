#include "D3D10RenderWindow.h"
#include "D3D10Renderer.h"
#include "D3D10RenderTexture.h"
#include "D3D10RenderCubeTexture.h"
#include "../Material/D3D10MakeShadow.h"

namespace Flagship
{
	D3D10RenderWindow::D3D10RenderWindow()
	{
		m_pD3D10SwapChain = NULL;
		m_pD3D10Device = NULL;
		m_pLDRSurfaceRT = NULL;
		m_pLDRSurfaceDS = NULL;
		m_pHDRSurfaceRT = NULL;
		m_pHDRSurfaceDS = NULL;

		m_pScreenVertexBuffer = NULL;
		m_pScreenInputLayout = NULL;

		m_pRenderer = new D3D10Renderer;
	}

	D3D10RenderWindow::~D3D10RenderWindow()
	{
		SAFE_RELEASE( m_pD3D10SwapChain );
		SAFE_RELEASE( m_pD3D10Device );

		SAFE_RELEASE( m_pLDRTextureRT );
		SAFE_RELEASE( m_pLDRSurfaceRT );
		SAFE_RELEASE( m_pLDRTextureDS );
		SAFE_RELEASE( m_pLDRSurfaceDS );
		SAFE_RELEASE( m_pHDRTextureRT );
		SAFE_RELEASE( m_pHDRSurfaceRT );
		SAFE_RELEASE( m_pHDRTextureDS );
		SAFE_RELEASE( m_pHDRSurfaceDS );

		SAFE_RELEASE( m_pScreenVertexBuffer );
		SAFE_RELEASE( m_pScreenInputLayout );

		SAFE_DELETE( m_pRenderer );
	}

	bool    D3D10RenderWindow::Initialize( bool bFullScreen, bool bHDR )
	{
		//  获取窗口大小
		RECT rc;
		GetClientRect( m_hWindowHandle, &rc );
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;
		m_uiWidth = width;
		m_uiHeight = height;

		// 创建设备
		DXGI_SWAP_CHAIN_DESC DescSC;
		ZeroMemory( &DescSC, sizeof(DescSC) );
		DescSC.BufferCount = 1;
		DescSC.BufferDesc.Width = width;
		DescSC.BufferDesc.Height = height;
		DescSC.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		DescSC.BufferDesc.RefreshRate.Numerator = 60;
		DescSC.BufferDesc.RefreshRate.Denominator = 1;
		DescSC.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		DescSC.OutputWindow = m_hWindowHandle;
		DescSC.SampleDesc.Count = 1;
		DescSC.SampleDesc.Quality = 0;
		DescSC.Windowed = ! bFullScreen;

		HRESULT hr;
		hr = D3D10CreateDeviceAndSwapChain( NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, &DescSC, &m_pD3D10SwapChain, &m_pD3D10Device );
		
		if( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D10RenderWindow::Initialize():D3D10CreateDeviceAndSwapChain() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		// 创建字体
		D3DX10CreateFont( m_pD3D10Device, 24, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
			L"Arial", &m_pFont );

		hr = m_pD3D10SwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), (LPVOID *) &m_pLDRTextureRT );
		if( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D10RenderWindow::Initialize():GetBuffer() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		// 创建渲染对象
		hr = m_pD3D10Device->CreateRenderTargetView( m_pLDRTextureRT, NULL, &m_pLDRSurfaceRT );

		if( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D10RenderWindow::Initialize():CreateRenderTargetView() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		D3D10_TEXTURE2D_DESC DescLDRTexDS;
		DescLDRTexDS.Width = width;
		DescLDRTexDS.Height = height;
		DescLDRTexDS.MipLevels = 1;
		DescLDRTexDS.ArraySize = 1;
		DescLDRTexDS.Format = DXGI_FORMAT_D32_FLOAT;
		DescLDRTexDS.SampleDesc.Count = 1;
		DescLDRTexDS.SampleDesc.Quality = 0;
		DescLDRTexDS.Usage = D3D10_USAGE_DEFAULT;
		DescLDRTexDS.BindFlags = D3D10_BIND_DEPTH_STENCIL;
		DescLDRTexDS.CPUAccessFlags = 0;
		DescLDRTexDS.MiscFlags = 0;

		hr = m_pD3D10Device->CreateTexture2D( &DescLDRTexDS, NULL, &m_pLDRTextureDS );

		if( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D10RenderWindow::Initialize():CreateTexture2D() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}
		
		hr = m_pD3D10Device->CreateDepthStencilView( m_pLDRTextureDS, NULL, &m_pLDRSurfaceDS );

		if( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D10RenderWindow::Initialize():CreateDepthStencilView() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		m_pD3D10Device->OMSetRenderTargets( 1, &m_pLDRSurfaceRT, m_pLDRSurfaceDS );
		
		// 设置视口
		D3D10_VIEWPORT vp;
		vp.Width = width;
		vp.Height = height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_pD3D10Device->RSSetViewports( 1, &vp );

		// HDR渲染对象
		m_bHDR = bHDR;
		if ( bHDR )
		{
			D3D10_TEXTURE2D_DESC DescHDRTexRT;
			DescHDRTexRT.Width = width;
			DescHDRTexRT.Height = height;
			DescHDRTexRT.MipLevels = 1;
			DescHDRTexRT.ArraySize = 1;
			DescHDRTexRT.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			DescHDRTexRT.SampleDesc.Count = 1;
			DescHDRTexRT.SampleDesc.Quality = 0;
			DescHDRTexRT.Usage = D3D10_USAGE_DEFAULT;
			DescHDRTexRT.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
			DescHDRTexRT.CPUAccessFlags = 0;
			DescHDRTexRT.MiscFlags = 0;

			hr = m_pD3D10Device->CreateTexture2D( &DescHDRTexRT, NULL, &m_pHDRTextureRT );

			if( FAILED( hr ) )
			{
				char szLog[10240];
				sprintf( szLog, "D3D10RenderWindow::Initialize():CreateTexture2D() Fail!" );
				LogManager::GetSingleton()->WriteLog( szLog );

				return false;
			}

			// 创建渲染对象
			hr = m_pD3D10Device->CreateRenderTargetView( m_pHDRTextureRT, NULL, &m_pHDRSurfaceRT );

			if( FAILED( hr ) )
			{
				char szLog[10240];
				sprintf( szLog, "D3D10RenderWindow::Initialize():CreateRenderTargetView() Fail!" );
				LogManager::GetSingleton()->WriteLog( szLog );

				return false;
			}

			D3D10_TEXTURE2D_DESC DescHDRTexDS;
			DescHDRTexDS.Width = width;
			DescHDRTexDS.Height = height;
			DescHDRTexDS.MipLevels = 1;
			DescHDRTexDS.ArraySize = 1;
			DescHDRTexDS.Format = DXGI_FORMAT_D32_FLOAT;
			DescHDRTexDS.SampleDesc.Count = 1;
			DescHDRTexDS.SampleDesc.Quality = 0;
			DescHDRTexDS.Usage = D3D10_USAGE_DEFAULT;
			DescHDRTexDS.BindFlags = D3D10_BIND_DEPTH_STENCIL;
			DescHDRTexDS.CPUAccessFlags = 0;
			DescHDRTexDS.MiscFlags = 0;

			hr = m_pD3D10Device->CreateTexture2D( &DescHDRTexDS, NULL, &m_pHDRTextureDS );

			if( FAILED( hr ) )
			{
				char szLog[10240];
				sprintf( szLog, "D3D10RenderWindow::Initialize():CreateTexture2D() Fail!" );
				LogManager::GetSingleton()->WriteLog( szLog );

				return false;
			}

			hr = m_pD3D10Device->CreateDepthStencilView( m_pHDRTextureDS, NULL, &m_pHDRSurfaceDS );

			if( FAILED( hr ) )
			{
				char szLog[10240];
				sprintf( szLog, "D3D10RenderWindow::Initialize():CreateDepthStencilView() Fail!" );
				LogManager::GetSingleton()->WriteLog( szLog );

				return false;
			}

			m_pD3D10Device->OMSetRenderTargets( 1, &m_pHDRSurfaceRT, m_pHDRSurfaceDS );
		}

		// Create a screen quad for all render to texture operations
		ScreenVertex svQuad[4];
		svQuad[0].vPosition = D3DXVECTOR4(-1.0f, 1.0f, 0.5f, 1.0f);
		svQuad[0].vTextureCoord = D3DXVECTOR2(0.0f, 0.0f);
		svQuad[1].vPosition = D3DXVECTOR4(1.0f, 1.0f, 0.5f, 1.0f);
		svQuad[1].vTextureCoord = D3DXVECTOR2(1.0f, 0.0f);
		svQuad[2].vPosition = D3DXVECTOR4(-1.0f, -1.0f, 0.5f, 1.0f);
		svQuad[2].vTextureCoord = D3DXVECTOR2(0.0f, 1.0f);
		svQuad[3].vPosition = D3DXVECTOR4(1.0f, -1.0f, 0.5f, 1.0f);
		svQuad[3].vTextureCoord = D3DXVECTOR2(1.0f, 1.0f);

		D3D10_BUFFER_DESC vbdesc =
		{
			4*sizeof(ScreenVertex),
			D3D10_USAGE_DEFAULT,
			D3D10_BIND_VERTEX_BUFFER,
			0,
			0
		};

		D3D10_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = svQuad;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		hr = m_pD3D10Device->CreateBuffer( &vbdesc, &InitData, &m_pScreenVertexBuffer );

		if( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D10RenderWindow::Initialize():CreateBuffer() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		D3D10_INPUT_ELEMENT_DESC TempDecl1 = { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 };
		m_pScreenInputElement[0] = TempDecl1;
		D3D10_INPUT_ELEMENT_DESC TempDecl2 = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D10_INPUT_PER_VERTEX_DATA, 0 };
		m_pScreenInputElement[1] = TempDecl2;

		RenderWindow::SetActiveRenderWindow( this );

		return true;
	}

	void    D3D10RenderWindow::RestoreRenderTarget()
	{
        m_pD3D10Device->OMSetRenderTargets( 1, &m_pLDRSurfaceRT, m_pLDRSurfaceDS );
	}

	ID3D10Texture2D *    D3D10RenderWindow::GetRenderTarget()
	{
		if ( m_bHDR )
		{
			return m_pHDRTextureRT;
		}

		return m_pLDRTextureRT;
	}

	ID3D10Texture2D *    D3D10RenderWindow::GetDepthStencil()
	{
		if ( m_bHDR )
		{
			return m_pHDRTextureDS;
		}

		return m_pLDRTextureDS;
	}

	ID3D10RenderTargetView *    D3D10RenderWindow::GetRenderTargetView()
	{
		if ( m_bHDR )
		{
			return m_pHDRSurfaceRT;
		}

		return m_pLDRSurfaceRT;
	}

	ID3D10DepthStencilView *    D3D10RenderWindow::GetDepthStencilView()
	{
		if ( m_bHDR )
		{
			return m_pHDRSurfaceDS;
		}

		return m_pLDRSurfaceDS;
	}

	ID3D10Buffer *    D3D10RenderWindow::GetScreenVertexBuffer()
	{
		return m_pScreenVertexBuffer;
	}

	ID3D10InputLayout *    D3D10RenderWindow::GetScreenVertexDeclaration( ID3D10EffectTechnique * pTech )
	{
		if ( m_pScreenInputLayout != NULL )
		{
			return m_pScreenInputLayout;
		}

		// 获取描述
		D3D10_PASS_DESC PassDesc;
		pTech->GetPassByIndex( 0 )->GetDesc( &PassDesc );

		// 创建InputLayout
		HRESULT hr = m_pD3D10Device->CreateInputLayout( m_pScreenInputElement, 2, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_pScreenInputLayout );
		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D10RenderWindow::GetScreenVertexDeclaration Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return NULL;
		}

		return m_pScreenInputLayout;
	}

	void    D3D10RenderWindow::Update()
	{
		if ( m_bHDR )
		{
			m_pD3D10Device->OMSetRenderTargets( 1, &m_pHDRSurfaceRT, m_pHDRSurfaceDS );
		}

		RenderWindow::Update();
	}

	void    D3D10RenderWindow::SaveRenderTexture( wstring szPath )
	{
		if ( m_bHDR )
		{
			D3DX10SaveTextureToFile( m_pHDRTextureRT, D3DX10_IFF_DDS, szPath.c_str() );
		}
		else
		{
			D3DX10SaveTextureToFile( m_pLDRTextureRT, D3DX10_IFF_DDS, szPath.c_str() );
		}
	}

	ID3D10Device *    D3D10RenderWindow::GetDevice()
	{
		return m_pD3D10Device;
	}

	IDXGISwapChain *    D3D10RenderWindow::GetSwapChain()
	{
		return m_pD3D10SwapChain;
	}

	ID3DX10Font *    D3D10RenderWindow::GetFont()
	{
		return m_pFont;
	}
}