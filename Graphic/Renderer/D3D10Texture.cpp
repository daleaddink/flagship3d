#include "D3D10Texture.h"
#include "D3D10RenderWindow.h"

namespace Flagship
{
	extern HINSTANCE g_hInstance;

	D3D10Texture::D3D10Texture()
	{
		m_pD3D10Texture = NULL;
		m_pD3D10ShaderResource = NULL;
		m_pD3D10RenderTarget = NULL;
		m_pD3D10DepthStencil = NULL;
		m_iClassType = Base::Texture_General;
	}

	D3D10Texture::~D3D10Texture()
	{
		SAFE_RELEASE( m_pD3D10Texture );
		SAFE_RELEASE( m_pD3D10ShaderResource );
		SAFE_RELEASE( m_pD3D10RenderTarget );
		SAFE_RELEASE( m_pD3D10DepthStencil );
	}

	ID3D10Texture2D *    D3D10Texture::GetImpliment()
	{
		return m_pD3D10Texture;
	}

	ID3D10ShaderResourceView *    D3D10Texture::GetShaderResourceView()
	{
		return m_pD3D10ShaderResource;
	}

	ID3D10RenderTargetView *    D3D10Texture::GetRenderTargetView()
	{
		return m_pD3D10RenderTarget;
	}

	ID3D10DepthStencilView *    D3D10Texture::GetDepthStencilView()
	{
		return m_pD3D10DepthStencil;
	}

	bool    D3D10Texture::CreateFromMemory()
	{
		// 获取D3D10设备指针
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 创建D3D10贴图对象
		D3DX10_IMAGE_LOAD_INFO LoadInfo;
		LoadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;

		ID3D10Resource * pResource;
		HRESULT hr = D3DX10CreateTextureFromMemory( pD3D10Device, m_kFileBuffer.GetPointer()
			, m_kFileBuffer.GetSize(), &LoadInfo, NULL, &pResource, NULL );
		pResource->QueryInterface( __uuidof( ID3D10Texture2D ), (LPVOID*)&m_pD3D10Texture );
		pD3D10Device->CreateShaderResourceView( m_pD3D10Texture, NULL, &m_pD3D10ShaderResource );
		SAFE_RELEASE( pResource );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			sprintf( szLog, "D3D10Texture::Cache() Fail! File:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;				 
		}

		return true;
	}

	bool    D3D10Texture::CreateDynamic( UINT uiWidth, UINT uiHeight, DWORD dwFormat )
	{
		// 获取D3D10设备指针
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 创建D3D10贴图对象
		D3D10_TEXTURE2D_DESC Desc;
		ZeroMemory( &Desc, sizeof( D3D10_TEXTURE2D_DESC ) );
		Desc.ArraySize = 1;
		Desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
		Desc.Usage = D3D10_USAGE_DYNAMIC;
		Desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		Desc.Format = (DXGI_FORMAT) dwFormat;
		Desc.Width = uiWidth;
		Desc.Height = uiHeight;
		Desc.MipLevels = 1;
		Desc.SampleDesc.Count = 1;

		HRESULT hr = pD3D10Device->CreateTexture2D( &Desc, NULL, &m_pD3D10Texture );
		pD3D10Device->CreateShaderResourceView( m_pD3D10Texture, NULL, &m_pD3D10ShaderResource );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D10Texture::CreateRenderTexture() Fail! Format:%d", (int)dwFormat );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D10Texture::CreateRenderTarget( UINT uiWidth, UINT uiHeight, DWORD dwFormat )
	{
		// 获取D3D10设备指针
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 创建D3D10贴图对象
		D3D10_TEXTURE2D_DESC Desc;
		ZeroMemory( &Desc, sizeof( D3D10_TEXTURE2D_DESC ) );
		Desc.Width = uiWidth;
		Desc.Height = uiHeight;
		Desc.MipLevels = 1;
		Desc.ArraySize = 1;
		Desc.Format = (DXGI_FORMAT) dwFormat;
		Desc.SampleDesc.Count = 1;
		Desc.SampleDesc.Quality = 0;
		Desc.Usage = D3D10_USAGE_DEFAULT;
		Desc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
		Desc.CPUAccessFlags = 0;
		Desc.MiscFlags = 0;
		HRESULT hr = pD3D10Device->CreateTexture2D( &Desc, NULL, &m_pD3D10Texture );

		D3D10_SHADER_RESOURCE_VIEW_DESC DescRV;
		DescRV.Format = Desc.Format;
		DescRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
		DescRV.Texture2D.MipLevels = 1;
		DescRV.Texture2D.MostDetailedMip = 0;
		pD3D10Device->CreateShaderResourceView( m_pD3D10Texture, &DescRV, &m_pD3D10ShaderResource );
		
		D3D10_RENDER_TARGET_VIEW_DESC DescRT;
		DescRT.Format = Desc.Format;
		DescRT.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
		DescRT.Texture2D.MipSlice = 0;
		pD3D10Device->CreateRenderTargetView( m_pD3D10Texture, &DescRT, &m_pD3D10RenderTarget );
		
		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D10Texture::CreateRenderTexture() Fail! Format:%d", (int)dwFormat );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D10Texture::CreateDepthStencil( UINT uiWidth, UINT uiHeight, DWORD dwFormat )
	{
		// 获取D3D10设备指针
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 创建D3D10贴图对象
		D3D10_TEXTURE2D_DESC Desc;
		Desc.Width = uiWidth;
		Desc.Height = uiHeight;
		Desc.MipLevels = 1;
		Desc.ArraySize = 1;
		Desc.Format = DXGI_FORMAT_D32_FLOAT;
		Desc.SampleDesc.Count = 1;
		Desc.SampleDesc.Quality = 0;
		Desc.Usage = D3D10_USAGE_DEFAULT;
		Desc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
		Desc.CPUAccessFlags = 0;
		Desc.MiscFlags = 0;

		HRESULT hr = pD3D10Device->CreateTexture2D( &Desc, NULL, &m_pD3D10Texture );
		pD3D10Device->CreateDepthStencilView( m_pD3D10Texture, NULL, &m_pD3D10DepthStencil );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D10Texture::CreateDepthStencil() Fail! Format:%d", (int)dwFormat );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D10Texture::ClearTexture()
	{
		if ( m_pD3D10RenderTarget == NULL )
		{
			return false;
		}

		// 获取D3D10设备指针
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		pD3D10Device->ClearRenderTargetView( m_pD3D10RenderTarget, ClearColor );

		return true;
	}

	bool    D3D10Texture::Lock( int& rPitch, void ** ppData )
	{
		D3D10_MAPPED_TEXTURE2D rect;
		HRESULT hr = m_pD3D10Texture->Map( 0, D3D10_MAP_WRITE_DISCARD, 0, &rect );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D10Texture::Lock() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		rPitch = rect.RowPitch;
		* ppData = rect.pData;

		return true;
	}

	void    D3D10Texture::UnLock()
	{
		m_pD3D10Texture->Unmap( 0 );
	}

	void    D3D10Texture::UnCache()
	{
		Texture::UnCache();
		SAFE_RELEASE( m_pD3D10Texture );
		SAFE_RELEASE( m_pD3D10ShaderResource );
		SAFE_RELEASE( m_pD3D10RenderTarget );
		SAFE_RELEASE( m_pD3D10DepthStencil );
	}
}