#include "D3D10VolumeTexture.h"
#include "D3D10RenderWindow.h"

namespace Flagship
{
	D3D10VolumeTexture::D3D10VolumeTexture()
	{
		m_pD3D10VolumeTexture = NULL;
		m_pD3D10ShaderResource = NULL;
		m_iClassType = Base::Texture_Volume;
	}

	D3D10VolumeTexture::~D3D10VolumeTexture()
	{
		SAFE_RELEASE( m_pD3D10VolumeTexture );
		SAFE_RELEASE( m_pD3D10ShaderResource );
	}

	ID3D10Texture3D *    D3D10VolumeTexture::GetImpliment()
	{
		return m_pD3D10VolumeTexture;
	}

	ID3D10ShaderResourceView *    D3D10VolumeTexture::GetShaderResourceView()
	{
		return m_pD3D10ShaderResource;
	}

	bool    D3D10VolumeTexture::ClearTexture()
	{
		return true;
	}

	bool    D3D10VolumeTexture::CreateFromMemory()
	{
		// 获取D3D10设备指针
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 创建D3D10贴图对象
		D3DX10_IMAGE_LOAD_INFO LoadInfo;
		LoadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;

		ID3D10Resource * pResource;
		HRESULT hr = D3DX10CreateTextureFromMemory( pD3D10Device, m_kFileBuffer.GetPointer()
			, m_kFileBuffer.GetSize(), &LoadInfo, NULL, &pResource, NULL );
		pResource->QueryInterface( __uuidof( ID3D10Texture3D ), (LPVOID*)&m_pD3D10VolumeTexture );
		pD3D10Device->CreateShaderResourceView( m_pD3D10VolumeTexture, NULL, &m_pD3D10ShaderResource );
		SAFE_RELEASE( pResource );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			sprintf( szLog, "D3D10VolumeTexture::CreateFromMemory() Fail! File:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	void    D3D10VolumeTexture::UnCache()
	{
		Texture::UnCache();
		SAFE_RELEASE( m_pD3D10VolumeTexture );
		SAFE_RELEASE( m_pD3D10ShaderResource );
	}
}