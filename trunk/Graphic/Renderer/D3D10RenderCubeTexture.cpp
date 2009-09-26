#include "D3D10RenderCubeTexture.h"
#include "D3D10RenderWindow.h"
#include "D3D10Renderer.h"
#include "D3D10Texture.h"
#include "D3D10CubeTexture.h"

#include "../Main/RenderTarget.h"
#include "../Main/DataCenter.h"

namespace Flagship
{
	D3D10RenderCubeTexture::D3D10RenderCubeTexture()
	{
		m_pRenderTarget = NULL;
		m_pDepthSurface = NULL;

		for ( int i = 0; i < Face_Max; i++ )
		{
			m_pRenderSurface[i] = NULL;
		}

		m_pRenderer = new D3D10Renderer;
	}

	D3D10RenderCubeTexture::~D3D10RenderCubeTexture()
	{
		for ( int i = 0; i < Face_Max; i++ )
		{
			SAFE_RELEASE( m_pRenderSurface[i] );
		}

		SAFE_RELEASE( m_pDepthSurface );
		SAFE_DELETE( m_pRenderTarget );

		SAFE_DELETE( m_pRenderer );
	}

	bool    D3D10RenderCubeTexture::Initialize( UINT uiSize, DWORD dwFormat )
	{
		// 获取D3D10设备指针
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 创建渲染贴图
		m_pRenderTarget = new D3D10CubeTexture;
		if ( ! m_pRenderTarget->CreateRenderTarget( uiSize, uiSize, dwFormat ) )
		{
			return false;
		}

		// 获取渲染表面
		D3D10_RENDER_TARGET_VIEW_DESC DescRT;
		DescRT.Format = (DXGI_FORMAT) dwFormat;
		DescRT.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2DARRAY;
		DescRT.Texture2DArray.FirstArraySlice = 0;
		DescRT.Texture2DArray.ArraySize = 1;
		DescRT.Texture2DArray.MipSlice = 0;

		HRESULT hr;
		D3D10CubeTexture * pRenderTarget = (D3D10CubeTexture *) m_pRenderTarget;
		for( int i = 0; i < 6; i++ )
		{
			DescRT.Texture2DArray.FirstArraySlice = i;
			hr = pD3D10Device->CreateRenderTargetView( pRenderTarget->GetImpliment(), &DescRT, &( m_pRenderSurface[i] ) );
		}
				
		// 创建深度模版表面
		m_pDepthStencil = new D3D10CubeTexture;
		if ( ! m_pDepthStencil->CreateDepthStencil( uiSize, uiSize, DXGI_FORMAT_D32_FLOAT ) )
		{
			return false;
		}

		// 获取深度模板表面
		D3D10_DEPTH_STENCIL_VIEW_DESC DescDS;
		DescDS.Format = DXGI_FORMAT_D32_FLOAT;
		DescDS.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2DARRAY;
		DescDS.Texture2DArray.FirstArraySlice = 0;
		DescDS.Texture2DArray.ArraySize = 1;
		DescDS.Texture2DArray.MipSlice = 0;

		D3D10CubeTexture * pDepthStencil = (D3D10CubeTexture *) m_pDepthStencil;
		hr = pD3D10Device->CreateDepthStencilView( pDepthStencil->GetImpliment(), &DescDS, &m_pDepthSurface );

		if( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D10RenderCubeTexture::Initialize():CreateDepthStencilView() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	void    D3D10RenderCubeTexture::SaveRenderTexture( wstring szPath )
	{
		D3D10CubeTexture * pRenderTarget = (D3D10CubeTexture *) m_pRenderTarget;
		D3DX10SaveTextureToFile( pRenderTarget->GetImpliment(), D3DX10_IFF_DDS, szPath.c_str() );
	}

	void    D3D10RenderCubeTexture::Update()
	{
		// 检测摄像机
		if ( m_pCamera == NULL )
		{
			return;
		}

		// 获取D3D10设备指针
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 获取旧的信息
		ID3D10RenderTargetView * pOldRenderSurface;
		ID3D10DepthStencilView * pOldDepthSurface;
		pD3D10Device->OMGetRenderTargets( 1, &pOldRenderSurface, &pOldDepthSurface );
		RenderTarget * pOldRenderTarget = GetActiveRenderTarget();
		Camera * pOldCamera = GetActiveCamera();

		// 设置渲染对象并渲染
		Vector4f vPos, vDir, vRight, vUp;
		Vector4f * pPos = DataCenter::GetSingleton()->GetVectorData( m_pCamera, Base::Class_Camera, CameraData::Vector4_Position )->Read();
		vPos = * pPos;

		// 渲染六个表面
		vDir = Vector4f( 1.0f, 0.0f, 0.0f, 0.0f );
		vRight = Vector4f( 0.0f, 0.0f, -1.0f, 0.0f );
		vUp = Vector4f( 0.0f, 1.0f, 0.0f, 0.0f );
		m_pCamera->SetView( vPos, vDir, vUp, vRight );
		pD3D10Device->OMSetRenderTargets( 1, &( m_pRenderSurface[Face_PositiveX] ), m_pDepthSurface );
		RenderTexture::Update();

		vDir = Vector4f( -1.0f, 0.0f, 0.0f, 0.0f );
		vRight = Vector4f( 0.0f, 0.0f, 1.0f, 0.0f );
		vUp = Vector4f( 0.0f, 1.0f, 0.0f, 0.0f );
		m_pCamera->SetView( vPos, vDir, vUp, vRight );
		pD3D10Device->OMSetRenderTargets( 1, &( m_pRenderSurface[Face_NegativeX] ), m_pDepthSurface );
		RenderTexture::Update();

		vDir = Vector4f( 0.0f, 1.0f, 0.0f, 0.0f );
		vRight = Vector4f( 1.0f, 0.0f, 0.0f, 0.0f );
		vUp = Vector4f( 0.0f, 0.0f, -1.0f, 0.0f );
		m_pCamera->SetView( vPos, vDir, vUp, vRight );
		pD3D10Device->OMSetRenderTargets( 1, &( m_pRenderSurface[Face_PositiveY] ), m_pDepthSurface );
		RenderTexture::Update();

		vDir = Vector4f( 0.0f, -1.0f, 0.0f, 0.0f );
		vRight = Vector4f( 1.0f, 0.0f, 0.0f, 0.0f );
		vUp = Vector4f( 0.0f, 0.0f, 1.0f, 0.0f );
		m_pCamera->SetView( vPos, vDir, vUp, vRight );
		pD3D10Device->OMSetRenderTargets( 1, &( m_pRenderSurface[Face_NegativeY] ), m_pDepthSurface );
		RenderTexture::Update();

		vDir = Vector4f( 0.0f, 0.0f, 1.0f, 0.0f );
		vRight = Vector4f( 1.0f, 0.0f, 0.0f, 0.0f );
		vUp = Vector4f( 0.0f, 1.0f, 0.0f, 0.0f );
		m_pCamera->SetView( vPos, vDir, vUp, vRight );
		pD3D10Device->OMSetRenderTargets( 1, &( m_pRenderSurface[Face_PositiveZ] ), m_pDepthSurface );
		RenderTexture::Update();

		vDir = Vector4f( 0.0f, 0.0f, -1.0f, 0.0f );
		vRight = Vector4f( -1.0f, 0.0f, 0.0f, 0.0f );
		vUp = Vector4f( 0.0f, 1.0f, 0.0f, 0.0f );
		m_pCamera->SetView( vPos, vDir, vUp, vRight );
		pD3D10Device->OMSetRenderTargets( 1, &( m_pRenderSurface[Face_NegativeZ] ), m_pDepthSurface );
		RenderTexture::Update();

		// 恢复旧的信息
		pD3D10Device->OMSetRenderTargets( 1, &pOldRenderSurface, pOldDepthSurface );
		SetActiveRenderTarget( pOldRenderTarget );
		SetActiveCamera( pOldCamera );

		SAFE_RELEASE( pOldRenderSurface );
		SAFE_RELEASE( pOldDepthSurface );
	}
}