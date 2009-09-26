#include "D3D10Renderer.h"
#include "D3D10RenderWindow.h"
#include "D3D10PMFMesh.h"
#include "D3D10SDKMesh.h"
#include "D3D10Material.h"
#include "D3D10Effect.h"
#include "D3D10RenderTexture.h"
#include "D3D10RenderCubeTexture.h"

namespace Flagship
{
	D3D10Renderer::D3D10Renderer( BYTE byRenderType )
		: Renderer( byRenderType )
	{

	}

	D3D10Renderer::~D3D10Renderer()
	{

	}

	bool    D3D10Renderer::BeginScene()
	{
		return true;
	}

	bool    D3D10Renderer::EndScene()
	{
		return true;
	}

	bool    D3D10Renderer::Clear(  Vector4f& vColor )
	{
		// 获取D3D10设备指针
		D3D10RenderWindow * pD3D10RenderWindow = ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow();
		ID3D10Device * pD3D10Device = pD3D10RenderWindow->GetDevice();

		// 清空缓存
		ID3D10RenderTargetView * pRenderSurface;
		ID3D10DepthStencilView * pDepthSurface;
		pD3D10Device->OMGetRenderTargets( 1, &pRenderSurface, &pDepthSurface );

		float ClearColor[4] = { vColor[0], vColor[1], vColor[2], vColor[3] };
		pD3D10Device->ClearRenderTargetView( pRenderSurface, ClearColor );
		pD3D10Device->ClearDepthStencilView( pDepthSurface, D3D10_CLEAR_DEPTH, 1.0, 0 );

		return true;
	}

	bool    D3D10Renderer::Present()
	{
		// 获取D3D10缓冲链
		D3D10RenderWindow * pD3D10RenderWindow = ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow();
		IDXGISwapChain * pD3D10SwapChain = pD3D10RenderWindow->GetSwapChain();

		// 交换缓冲
		HRESULT hr = pD3D10SwapChain->Present( 0, 0 );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D10Renderer::Present() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D10Renderer::SetBlendEnable( bool bBlend )
	{
		// 获取D3D10设备指针
		D3D10RenderWindow * pD3D10RenderWindow = ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow();
		ID3D10Device * pD3D10Device = pD3D10RenderWindow->GetDevice();

		return true;
	}

	bool    D3D10Renderer::RenderSingle( Material * pMaterial, Resource * pResource )
	{
		// 获取D3D9设备指针
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 开始材质
		UINT uiPass;
		if ( ! pMaterial->Begin( &uiPass ) )
		{
			return false;
		}

		// 渲染资源
		ID3D10Buffer * pVertexBuffer;
		ID3D10Buffer * pIndexBuffer;
		ID3D10InputLayout * pVertexDecl;
		DWORD dwNumTriangle;
		DWORD dwNumVertex;
		DWORD dwVertexSize;
		DXGI_FORMAT dwIndexFormat;

		switch ( pResource->GetClassType() )
		{
		case Base::Mesh_PMFMesh:
			{
				// 获取模型信息
				D3D10PMFMesh * pMesh = ( D3D10PMFMesh * ) pResource;
				D3D10Material * pD3D10Material = (D3D10Material *) pMaterial;

				pVertexBuffer = pMesh->GetVertexBuffer();
				pIndexBuffer = pMesh->GetIndexBuffer();
				pVertexDecl = pMesh->GetVertexDeclaration( pD3D10Material->GetTechKey(), pD3D10Material->GetTechnique() );
				dwNumTriangle = pMesh->GetNumTriangle();
				dwNumVertex = pMesh->GetNumVertex();
				dwVertexSize = pMesh->GetVertexSize();
				dwIndexFormat = pMesh->GetIndexFormat();

				break;
			}
		case Base::Mesh_SDKMesh:
			{
				// 获取模型信息
				D3D10SDKMesh * pMesh = ( D3D10SDKMesh * ) pResource;
				D3D10Material * pD3D10Material = (D3D10Material *) pMaterial;

				pVertexBuffer = pMesh->GetVertexBuffer();
				pIndexBuffer = pMesh->GetIndexBuffer();
				pVertexDecl = pMesh->GetVertexDeclaration( pD3D10Material->GetTechKey(), pD3D10Material->GetTechnique() );
				dwNumTriangle = pMesh->GetNumTriangle();
				dwNumVertex = pMesh->GetNumVertex();
				dwVertexSize = pMesh->GetVertexSize();
				dwIndexFormat = pMesh->GetIndexFormat();

				break;
			}
		default:
			break;
		}

		pD3D10Device->IASetInputLayout( pVertexDecl );

		UINT uiStride = dwVertexSize;
		UINT uiOffset = 0;
		pD3D10Device->IASetVertexBuffers( 0, 1, &pVertexBuffer, &uiStride, &uiOffset );
		pD3D10Device->IASetIndexBuffer( pIndexBuffer, dwIndexFormat, 0 );
		pD3D10Device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		for ( int i = 0; i < (int)uiPass; i++ )
		{
			if ( pMaterial->BeginPass( i ) )
			{
				pD3D10Device->DrawIndexed( dwNumTriangle * 3, 0, 0 );
				pMaterial->EndPass();
			}
		}

		// 结束材质
		pMaterial->End();

		return true;
	}

	bool    D3D10Renderer::RenderQuad( Effect * pEffect, float fLeftU, float fTopV, float fRightU, float fBottomV )
	{
		// 获取D3D10设备指针
		D3D10RenderWindow * pD3D10RenderWindow = ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow();
		ID3D10Device * pD3D10Device = pD3D10RenderWindow->GetDevice();

		// Save the Old viewport
		D3D10_VIEWPORT vpOld[D3D10_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
		UINT nViewPorts = 1;
		pD3D10Device->RSGetViewports( &nViewPorts, vpOld );

		// 获取渲染对象长宽
		ID3D10RenderTargetView * pRenderTarget;
		pD3D10Device->OMGetRenderTargets( 1, &pRenderTarget, NULL );
		ID3D10Texture2D * pRenderTexture;
		pRenderTarget->GetResource( (ID3D10Resource **) &pRenderTexture );

		D3D10_TEXTURE2D_DESC DescRT;
		pRenderTexture->GetDesc( &DescRT );
		SAFE_RELEASE( pRenderTarget );
		SAFE_RELEASE( pRenderTexture );

		// Setup the viewport to match the backbuffer
		D3D10_VIEWPORT vp;
		vp.Width = (UINT) DescRT.Width;
		vp.Height = (UINT) DescRT.Height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		pD3D10Device->RSSetViewports( 1, &vp );
		
		UINT strides = sizeof(ScreenVertex);
		UINT offsets = 0;
		ID3D10Buffer * pBuffers[1] = { pD3D10RenderWindow->GetScreenVertexBuffer() };
		ID3D10EffectTechnique * pTech = ( (D3D10Effect *) pEffect )->GetTechnique();

		pD3D10Device->IASetInputLayout( pD3D10RenderWindow->GetScreenVertexDeclaration( pTech ) );
		pD3D10Device->IASetVertexBuffers( 0, 1, pBuffers, &strides, &offsets );
		pD3D10Device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

		D3D10_TECHNIQUE_DESC techDesc;
		pTech->GetDesc(&techDesc);

		for( UINT uiPass=0; uiPass < techDesc.Passes; uiPass++ )
		{
			pTech->GetPassByIndex( uiPass )->Apply( 0 );
			pD3D10Device->Draw( 4, 0 );
		}

		// Restore the Old viewport
		pD3D10Device->RSSetViewports( nViewPorts, vpOld );

		return true;
	}

	bool    D3D10Renderer::RenderText( int iX, int iY, Vector4f& vColor, wstring& szText )
	{
		// 设置参数
		RECT rc;
		SetRect( &rc, iX, iY, 0, 0 );
		D3DXCOLOR color( vColor[0], vColor[1], vColor[2], vColor[3] );

		// 获取字体
		ID3DX10Font * pFont = ( (D3D10RenderWindow *) RenderWindow::GetActiveRenderWindow() )->GetFont();

		// 渲染文字
		HRESULT hr = pFont->DrawText( NULL, szText.c_str(), -1, &rc, DT_NOCLIP, color );

		return true;
	}
}