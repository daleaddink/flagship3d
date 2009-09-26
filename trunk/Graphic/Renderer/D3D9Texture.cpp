#include "D3D9Texture.h"
#include "D3D9RenderWindow.h"

namespace Flagship
{
	extern HINSTANCE g_hInstance;

	D3D9Texture::D3D9Texture()
	{
		m_pD3D9Texture = NULL;
		m_iClassType = Base::Texture_General;
	}

	D3D9Texture::~D3D9Texture()
	{
		SAFE_RELEASE( m_pD3D9Texture );
	}

	LPDIRECT3DTEXTURE9    D3D9Texture::GetImpliment()
	{
		return m_pD3D9Texture;
	}

	bool    D3D9Texture::CreateFromMemory()
	{
		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();
		// 创建D3D9贴图对象
		HRESULT hr = D3DXCreateTextureFromFileInMemory( pD3D9Device, m_kFileBuffer.GetPointer(), m_kFileBuffer.GetSize(), &m_pD3D9Texture );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			sprintf( szLog, "D3D9Texture::Cache() Fail! File:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;				 
		}

		return true;
	}

	bool    D3D9Texture::CreateDynamic( UINT uiWidth, UINT uiHeight, DWORD dwFormat )
	{
		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();
		// 创建D3D9贴图对象
		HRESULT hr = D3DXCreateTexture( pD3D9Device, uiWidth, uiHeight, 1, D3DUSAGE_DYNAMIC, (D3DFORMAT) dwFormat, D3DPOOL_DEFAULT, &m_pD3D9Texture );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D9Texture::CreateDynamic() Fail! Format:%d", (int)dwFormat );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D9Texture::CreateRenderTarget( UINT uiWidth, UINT uiHeight, DWORD dwFormat )
	{
		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();
		// 创建D3D9贴图对象
		HRESULT hr = D3DXCreateTexture( pD3D9Device, uiWidth, uiHeight, 1, D3DUSAGE_RENDERTARGET, (D3DFORMAT) dwFormat, D3DPOOL_DEFAULT, &m_pD3D9Texture );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D9Texture::CreateRenderTexture() Fail! Format:%d", (int)dwFormat );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D9Texture::CreateDepthStencil( UINT uiWidth, UINT uiHeight, DWORD dwFormat )
	{
		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();
		// 创建D3D9贴图对象
		HRESULT hr = D3DXCreateTexture( pD3D9Device, uiWidth, uiHeight, 1, D3DUSAGE_DEPTHSTENCIL, (D3DFORMAT) dwFormat, D3DPOOL_DEFAULT, &m_pD3D9Texture );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D9Texture::CreateDepthStencil() Fail! Format:%d", (int)dwFormat );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D9Texture::ClearTexture()
	{
		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();
		LPDIRECT3DSURFACE9 pSurface = NULL;

		m_pD3D9Texture->GetSurfaceLevel( 0, &pSurface );
    	pD3D9Device->ColorFill( pSurface, NULL, D3DCOLOR_ARGB(0, 0, 0, 0) );
		SAFE_RELEASE( pSurface );

		return true;
	}

	bool    D3D9Texture::Lock( int& rPitch, void ** ppData )
	{
		D3DLOCKED_RECT rect;
		HRESULT hr = m_pD3D9Texture->LockRect( 0, &rect, NULL, D3DLOCK_DISCARD );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D9Texture::Lock() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		rPitch = rect.Pitch;
		* ppData = rect.pBits;

		return true;
	}

	void    D3D9Texture::UnLock()
	{
		m_pD3D9Texture->UnlockRect( 0 );
	}

	void    D3D9Texture::UnCache()
	{
		Texture::UnCache();
		SAFE_RELEASE( m_pD3D9Texture );
	}
}