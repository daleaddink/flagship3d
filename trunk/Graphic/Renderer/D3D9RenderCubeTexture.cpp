#include "D3D9RenderCubeTexture.h"
#include "D3D9RenderWindow.h"
#include "D3D9Renderer.h"
#include "D3D9CubeTexture.h"

#include "../Main/RenderTarget.h"
#include "../Main/DataCenter.h"

namespace Flagship
{
	D3D9RenderCubeTexture::D3D9RenderCubeTexture()
	{
		m_pRenderTarget = NULL;
		m_pDepthSurface = NULL;

		for ( int i = 0; i < Face_Max; i++ )
		{
			m_pRenderSurface[i] = NULL;
		}

		m_pRenderer = new D3D9Renderer;
	}

	D3D9RenderCubeTexture::~D3D9RenderCubeTexture()
	{
		for ( int i = 0; i < Face_Max; i++ )
		{
			SAFE_RELEASE( m_pRenderSurface[i] );
		}

		SAFE_RELEASE( m_pDepthSurface );
		SAFE_DELETE( m_pRenderTarget );

		SAFE_DELETE( m_pRenderer );
	}

	bool    D3D9RenderCubeTexture::Initialize( UINT uiSize, DWORD dwFormat )
	{
		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 创建渲染贴图
		m_pRenderTarget = new D3D9CubeTexture;
		if ( ! m_pRenderTarget->CreateRenderTarget( uiSize, uiSize, dwFormat ) )
		{
			return false;
		}

		// 获取渲染表面
		D3D9CubeTexture * pRenderTexture = (D3D9CubeTexture *) m_pRenderTarget;
		pRenderTexture->GetImpliment()->GetCubeMapSurface( D3DCUBEMAP_FACE_POSITIVE_X, 0, &m_pRenderSurface[Face_PositiveX] );
		pRenderTexture->GetImpliment()->GetCubeMapSurface( D3DCUBEMAP_FACE_NEGATIVE_X, 0, &m_pRenderSurface[Face_NegativeX] );
		pRenderTexture->GetImpliment()->GetCubeMapSurface( D3DCUBEMAP_FACE_POSITIVE_Y, 0, &m_pRenderSurface[Face_PositiveY] );
		pRenderTexture->GetImpliment()->GetCubeMapSurface( D3DCUBEMAP_FACE_NEGATIVE_Y, 0, &m_pRenderSurface[Face_NegativeY] );
		pRenderTexture->GetImpliment()->GetCubeMapSurface( D3DCUBEMAP_FACE_POSITIVE_Z, 0, &m_pRenderSurface[Face_PositiveZ] );
		pRenderTexture->GetImpliment()->GetCubeMapSurface( D3DCUBEMAP_FACE_NEGATIVE_Z, 0, &m_pRenderSurface[Face_NegativeZ] );

		// 创建深度模版表面
		pD3D9Device->CreateDepthStencilSurface( uiSize, uiSize, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, true, &m_pDepthSurface, NULL );

		return true;
	}

	void    D3D9RenderCubeTexture::SaveRenderTexture( wstring szPath )
	{
		D3D9CubeTexture * pRenderTexture = (D3D9CubeTexture *) m_pRenderTarget;
		D3DXSaveTextureToFile( szPath.c_str(), D3DXIFF_DDS, pRenderTexture->GetImpliment(), NULL );
	}

	void    D3D9RenderCubeTexture::Update()
	{
		// 检测摄像机
		if ( m_pCamera == NULL )
		{
			return;
		}

		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 获取旧的信息
		LPDIRECT3DSURFACE9 pOldRenderSurface;
		LPDIRECT3DSURFACE9 pOldDepthSurface;
		pD3D9Device->GetRenderTarget( 0, &pOldRenderSurface );
		pD3D9Device->GetDepthStencilSurface( &pOldDepthSurface );
		RenderTarget * pOldRenderTarget = GetActiveRenderTarget();
		Camera * pOldCamera = GetActiveCamera();
		
		// 设置深度模版表面
		pD3D9Device->SetDepthStencilSurface( m_pDepthSurface );

		// 设置渲染对象并渲染
		Vector4f vPos, vDir, vRight, vUp;
		Vector4f * pPos = DataCenter::GetSingleton()->GetVectorData( m_pCamera, Base::Class_Camera, CameraData::Vector4_Position )->Read();
		vPos = * pPos;

		// 渲染六个表面
		vDir = Vector4f( 1.0f, 0.0f, 0.0f, 0.0f );
		vRight = Vector4f( 0.0f, 0.0f, -1.0f, 0.0f );
		vUp = Vector4f( 0.0f, 1.0f, 0.0f, 0.0f );
		m_pCamera->SetView( vPos, vDir, vUp, vRight );
		pD3D9Device->SetRenderTarget( 0, m_pRenderSurface[Face_PositiveX] );
		RenderTexture::Update();

		vDir = Vector4f( -1.0f, 0.0f, 0.0f, 0.0f );
		vRight = Vector4f( 0.0f, 0.0f, 1.0f, 0.0f );
		vUp = Vector4f( 0.0f, 1.0f, 0.0f, 0.0f );
		m_pCamera->SetView( vPos, vDir, vUp, vRight );
		pD3D9Device->SetRenderTarget( 0, m_pRenderSurface[Face_NegativeX] );
		RenderTexture::Update();

		vDir = Vector4f( 0.0f, 1.0f, 0.0f, 0.0f );
		vRight = Vector4f( 1.0f, 0.0f, 0.0f, 0.0f );
		vUp = Vector4f( 0.0f, 0.0f, -1.0f, 0.0f );
		m_pCamera->SetView( vPos, vDir, vUp, vRight );
		pD3D9Device->SetRenderTarget( 0, m_pRenderSurface[Face_PositiveY] );
		RenderTexture::Update();

		vDir = Vector4f( 0.0f, -1.0f, 0.0f, 0.0f );
		vRight = Vector4f( 1.0f, 0.0f, 0.0f, 0.0f );
		vUp = Vector4f( 0.0f, 0.0f, 1.0f, 0.0f );
		m_pCamera->SetView( vPos, vDir, vUp, vRight );
		pD3D9Device->SetRenderTarget( 0, m_pRenderSurface[Face_NegativeY] );
		RenderTexture::Update();

		vDir = Vector4f( 0.0f, 0.0f, 1.0f, 0.0f );
		vRight = Vector4f( 1.0f, 0.0f, 0.0f, 0.0f );
		vUp = Vector4f( 0.0f, 1.0f, 0.0f, 0.0f );
		m_pCamera->SetView( vPos, vDir, vUp, vRight );
		pD3D9Device->SetRenderTarget( 0, m_pRenderSurface[Face_PositiveZ] );
		RenderTexture::Update();

		vDir = Vector4f( 0.0f, 0.0f, -1.0f, 0.0f );
		vRight = Vector4f( -1.0f, 0.0f, 0.0f, 0.0f );
		vUp = Vector4f( 0.0f, 1.0f, 0.0f, 0.0f );
		m_pCamera->SetView( vPos, vDir, vUp, vRight );
		pD3D9Device->SetRenderTarget( 0, m_pRenderSurface[Face_NegativeZ] );
		RenderTexture::Update();

		// 恢复旧的信息
		pD3D9Device->SetRenderTarget( 0, pOldRenderSurface );
		pD3D9Device->SetDepthStencilSurface( pOldDepthSurface );
		SetActiveRenderTarget( pOldRenderTarget );
		SetActiveCamera( pOldCamera );

		SAFE_RELEASE( pOldRenderSurface );
		SAFE_RELEASE( pOldDepthSurface );
	}
}