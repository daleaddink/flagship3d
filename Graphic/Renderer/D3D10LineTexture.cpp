#include "D3D10LineTexture.h"
#include "D3D10RenderWindow.h"

namespace Flagship
{
	D3D10LineTexture::D3D10LineTexture()
	{
		m_pD3D10LineTexture = NULL;
		m_pD3D10ShaderResource = NULL;
		m_iClassType = Base::Texture_Line;
	}

	D3D10LineTexture::~D3D10LineTexture()
	{
		SAFE_RELEASE( m_pD3D10LineTexture );
		SAFE_RELEASE( m_pD3D10ShaderResource );
    }

	ID3D10Texture1D *    D3D10LineTexture::GetImpliment()
	{
		return m_pD3D10LineTexture;
	}

	ID3D10ShaderResourceView *    D3D10LineTexture::GetShaderResourceView()
	{
		return m_pD3D10ShaderResource;
	}

	bool    D3D10LineTexture::CreateFromMemory()
	{
		// 获取D3D10设备指针
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 创建D3D10贴图对象
		D3DX10_IMAGE_LOAD_INFO LoadInfo;
		LoadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;

		ID3D10Resource * pResource;
		HRESULT hr = D3DX10CreateTextureFromMemory( pD3D10Device, m_kFileBuffer.GetPointer()
			, m_kFileBuffer.GetSize(), &LoadInfo, NULL, &pResource, NULL );
		pResource->QueryInterface( __uuidof( ID3D10Texture1D ), (LPVOID*)&m_pD3D10LineTexture );
		pD3D10Device->CreateShaderResourceView( m_pD3D10LineTexture, NULL, &m_pD3D10ShaderResource );
		SAFE_RELEASE( pResource );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			sprintf( szLog, "D3D10LineTexture::CreateFromMemory() Fail! File:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D10LineTexture::CreateDynamic( UINT uiWidth, UINT uiHeight, DWORD dwFormat )
	{
		// 获取D3D10设备指针
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 创建D3D10贴图对象
		D3D10_TEXTURE1D_DESC Desc;
		Desc.Width = uiWidth;
		Desc.MipLevels = 1;
		Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		Desc.Usage = D3D10_USAGE_DYNAMIC;
		Desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
		Desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		Desc.MiscFlags = 0;
		Desc.ArraySize = 1;

		HRESULT hr = pD3D10Device->CreateTexture1D( &Desc, NULL, &m_pD3D10LineTexture );
		pD3D10Device->CreateShaderResourceView( m_pD3D10LineTexture, NULL, &m_pD3D10ShaderResource );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D10LineTexture::CreateRenderTexture() Fail! Format:%d", (int)dwFormat );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D10LineTexture::ClearTexture()
	{
		return true;
	}

	bool    D3D10LineTexture::Lock( int& rPitch, void ** ppData )
	{
		HRESULT hr = m_pD3D10LineTexture->Map( 0, D3D10_MAP_WRITE_DISCARD, 0, ppData );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D10LineTexture::Lock() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	void    D3D10LineTexture::UnLock()
	{
		m_pD3D10LineTexture->Unmap( 0 );
	}

	void    D3D10LineTexture::UnCache()
	{
		Texture::UnCache();
		SAFE_RELEASE( m_pD3D10LineTexture );
		SAFE_RELEASE( m_pD3D10ShaderResource );
	}
}