#include "D3D10CubeTexture.h"
#include "D3D10RenderWindow.h"

namespace Flagship
{
	D3D10CubeTexture::D3D10CubeTexture()
	{
		m_pD3D10CubeTexture = NULL;
		m_pD3D10ShaderResource = NULL;
		m_iClassType = Base::Texture_Cube;
	}

	D3D10CubeTexture::~D3D10CubeTexture()
	{
		SAFE_RELEASE( m_pD3D10CubeTexture );
		SAFE_RELEASE( m_pD3D10ShaderResource );
	}

	ID3D10Texture2D *    D3D10CubeTexture::GetImpliment()
	{
		return m_pD3D10CubeTexture;
	}

	ID3D10ShaderResourceView *    D3D10CubeTexture::GetShaderResourceView()
	{
		return m_pD3D10ShaderResource;
	}

	bool    D3D10CubeTexture::CreateFromMemory()
	{
		// 获取D3D10设备指针
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 创建D3D10贴图对象
		D3DX10_IMAGE_LOAD_INFO LoadInfo;
		LoadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
		LoadInfo.MiscFlags = D3D10_RESOURCE_MISC_TEXTURECUBE;

		ID3D10Resource * pResource;
		HRESULT hr = D3DX10CreateTextureFromMemory( pD3D10Device, m_kFileBuffer.GetPointer()
			, m_kFileBuffer.GetSize(), &LoadInfo, NULL, &pResource, NULL );
		pResource->QueryInterface( __uuidof( ID3D10Texture2D ), (LPVOID*)&m_pD3D10CubeTexture );
		pD3D10Device->CreateShaderResourceView( m_pD3D10CubeTexture, NULL, &m_pD3D10ShaderResource );
		SAFE_RELEASE( pResource );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			sprintf( szLog, "D3D10CubeTexture::CreateFromMemory() Fail! File:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D10CubeTexture::CreateRenderTarget( UINT uiWidth, UINT uiHeight, DWORD dwFormat )
	{
		// 获取D3D10设备指针
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 创建D3D10贴图对象
		D3D10_TEXTURE2D_DESC Desc;
		ZeroMemory( &Desc, sizeof( D3D10_TEXTURE2D_DESC ) );
		Desc.ArraySize = 6;
		Desc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
		Desc.Usage = D3D10_USAGE_DEFAULT;
		Desc.Format = (DXGI_FORMAT) dwFormat;
		Desc.Width = uiWidth;
		Desc.Height = uiHeight;
		Desc.MipLevels = 1;
		Desc.SampleDesc.Count = 1;
		Desc.MiscFlags = D3D10_RESOURCE_MISC_TEXTURECUBE;

		HRESULT hr = pD3D10Device->CreateTexture2D( &Desc, NULL, &m_pD3D10CubeTexture );
		pD3D10Device->CreateShaderResourceView( m_pD3D10CubeTexture, NULL, &m_pD3D10ShaderResource );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D10CubeTexture::CreateRenderTexture() Fail! Format:%d", (int)dwFormat );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D10CubeTexture::CreateDepthStencil( UINT uiWidth, UINT uiHeight, DWORD dwFormat )
	{
		// 获取D3D10设备指针
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 创建D3D10贴图对象
		D3D10_TEXTURE2D_DESC Desc;
		ZeroMemory( &Desc, sizeof( D3D10_TEXTURE2D_DESC ) );
		Desc.MipLevels = 1;
		Desc.ArraySize = 6;
		Desc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
		Desc.Usage = D3D10_USAGE_DEFAULT;
		Desc.Format = (DXGI_FORMAT) dwFormat;
		Desc.Width = uiWidth;
		Desc.Height = uiHeight;
		Desc.MipLevels = 1;
		Desc.SampleDesc.Count = 1;
		Desc.SampleDesc.Quality = 0;
		Desc.MiscFlags = D3D10_RESOURCE_MISC_TEXTURECUBE;

		HRESULT hr = pD3D10Device->CreateTexture2D( &Desc, NULL, &m_pD3D10CubeTexture );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D10CubeTexture::CreateDepthStencil() Fail! Format:%d", (int)dwFormat );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D10CubeTexture::ClearTexture()
	{
		return true;
	}

	void    D3D10CubeTexture::UnCache()
	{
		Texture::UnCache();
		SAFE_RELEASE( m_pD3D10CubeTexture );
		SAFE_RELEASE( m_pD3D10ShaderResource );
	}
}