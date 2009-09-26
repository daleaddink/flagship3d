#include "D3D9VolumeTexture.h"
#include "D3D9RenderWindow.h"

namespace Flagship
{
	D3D9VolumeTexture::D3D9VolumeTexture()
	{
		m_pD3D9VolumeTexture = NULL;
		m_iClassType = Base::Texture_Volume;
	}

	D3D9VolumeTexture::~D3D9VolumeTexture()
	{
		SAFE_RELEASE( m_pD3D9VolumeTexture );
	}

	LPDIRECT3DVOLUMETEXTURE9    D3D9VolumeTexture::GetImpliment()
	{
		return m_pD3D9VolumeTexture;
	}

	bool    D3D9VolumeTexture::CreateFromMemory()
	{
		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();
		// 创建D3D9贴图对象
		HRESULT hr = D3DXCreateVolumeTextureFromFileInMemory( pD3D9Device, m_kFileBuffer.GetPointer(), m_kFileBuffer.GetSize(), &m_pD3D9VolumeTexture );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			sprintf( szLog, "D3D9VolumeTexture::Cache() Fail! File:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;				 
		}

		return true;
	}

	bool    D3D9VolumeTexture::ClearTexture()
	{
		return true;
	}

	void    D3D9VolumeTexture::UnCache()
	{
		Texture::UnCache();
		SAFE_RELEASE( m_pD3D9VolumeTexture );
	}
}