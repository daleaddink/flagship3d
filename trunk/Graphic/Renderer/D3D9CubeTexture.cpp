#include "D3D9CubeTexture.h"
#include "D3D9RenderWindow.h"

namespace Flagship
{
	D3D9CubeTexture::D3D9CubeTexture()
	{
		m_pD3D9CubeTexture = NULL;
		m_iClassType = Base::Texture_Cube;
	}

	D3D9CubeTexture::~D3D9CubeTexture()
	{
		SAFE_RELEASE( m_pD3D9CubeTexture );
	}

	LPDIRECT3DCUBETEXTURE9    D3D9CubeTexture::GetImpliment()
	{
		return m_pD3D9CubeTexture;
	}

	bool    D3D9CubeTexture::CreateFromMemory()
	{
		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();
		// 创建D3D9贴图对象
		HRESULT hr = D3DXCreateCubeTextureFromFileInMemory( pD3D9Device, m_kFileBuffer.GetPointer(), m_kFileBuffer.GetSize(), &m_pD3D9CubeTexture );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			sprintf( szLog, "D3D9CubeTexture::Cache() Fail! File:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;				 
		}

		return true;
	}

	bool    D3D9CubeTexture::CreateRenderTarget( UINT uiWidth, UINT uiHeight, DWORD dwFormat )
	{
		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();
		// 创建D3D9贴图对象
		HRESULT hr = D3DXCreateCubeTexture( pD3D9Device, uiWidth, 1, D3DUSAGE_RENDERTARGET, (D3DFORMAT) dwFormat, D3DPOOL_DEFAULT, &m_pD3D9CubeTexture );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D9CubeTexture::CreateRenderTexture() Fail! Format:%d", (int)dwFormat );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;				 
		}

		return true;
	}

	bool    D3D9CubeTexture::ClearTexture()
	{
		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();
		LPDIRECT3DSURFACE9 pSurface[6];

		m_pD3D9CubeTexture->GetCubeMapSurface( D3DCUBEMAP_FACE_POSITIVE_X, 0, &pSurface[0] );
		m_pD3D9CubeTexture->GetCubeMapSurface( D3DCUBEMAP_FACE_NEGATIVE_X, 0, &pSurface[1] );
		m_pD3D9CubeTexture->GetCubeMapSurface( D3DCUBEMAP_FACE_POSITIVE_Y, 0, &pSurface[2] );
		m_pD3D9CubeTexture->GetCubeMapSurface( D3DCUBEMAP_FACE_NEGATIVE_Y, 0, &pSurface[3] );
		m_pD3D9CubeTexture->GetCubeMapSurface( D3DCUBEMAP_FACE_POSITIVE_Z, 0, &pSurface[4] );
		m_pD3D9CubeTexture->GetCubeMapSurface( D3DCUBEMAP_FACE_NEGATIVE_Z, 0, &pSurface[5] );

		for ( int i = 0; i < 6; i++ )
		{
			pD3D9Device->ColorFill( pSurface[i], NULL, D3DCOLOR_ARGB(0, 0, 0, 0) );
			SAFE_RELEASE( pSurface[i] );
		}

		return true;
	}

	void    D3D9CubeTexture::UnCache()
	{
		Texture::UnCache();
		SAFE_RELEASE( m_pD3D9CubeTexture );
	}
}