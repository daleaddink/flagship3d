#include "D3D9Renderer.h"
#include "D3D9RenderWindow.h"
#include "D3D9XMesh.h"
#include "D3D9PMFMesh.h"
#include "D3D9OgreMesh.h"
#include "D3D9TerrainTile.h"
#include "D3D9RenderTexture.h"
#include "D3D9RenderCubeTexture.h"
#include "../Main/Material.h"

namespace Flagship
{
	D3D9Renderer::D3D9Renderer( BYTE byRenderType )
		: Renderer( byRenderType )
	{

	}

	D3D9Renderer::~D3D9Renderer()
	{

	}

	bool    D3D9Renderer::BeginScene()
	{
		// ��ȡD3D9�豸ָ��
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// ��ʼ��Ⱦ
		HRESULT hr = pD3D9Device->BeginScene();

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D9Renderer::BeginScene() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D9Renderer::EndScene()
	{
		// ��ȡD3D9�豸ָ��
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();
		HRESULT hr = pD3D9Device->EndScene();

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D9Renderer::EndScene() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;				
		}

		return true;
	}

	bool    D3D9Renderer::Clear( Vector4f& vColor )
	{
		// ��ȡD3D9�豸ָ��
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// ��ջ���
		HRESULT hr = pD3D9Device->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, D3DCOLOR_COLORVALUE(vColor[0], vColor[1], vColor[2], vColor[3]), 1.0f, 0 );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D9Renderer::Clear() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D9Renderer::Present()
	{
		// ��ȡD3D9�豸ָ��
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();
		HRESULT hr = pD3D9Device->Present( NULL, NULL, NULL, NULL );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D9Renderer::Present() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D9Renderer::RenderSingle( Material * pMaterial, Resource * pResource )
	{
		// ��ȡD3D9�豸ָ��
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// ��ʼ����
		UINT uiPass;
		if ( ! pMaterial->Begin( &uiPass ) )
		{
			return false;
		}

		// ��Ⱦ��Դ
		LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;
		LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
		LPDIRECT3DVERTEXDECLARATION9 pVertexDecl;
		DWORD dwNumTriangle;
		DWORD dwNumVertex;
		DWORD dwVertexSize;

		switch ( pResource->GetClassType() )
		{
		case Base::Mesh_XMesh:
			{
				// ��ȡģ����Ϣ
				D3D9XMesh * pMesh = ( D3D9XMesh * ) pResource;
				pVertexBuffer = pMesh->GetVertexBuffer();
				pIndexBuffer = pMesh->GetIndexBuffer();
				pVertexDecl = pMesh->GetVertexDeclaration();
				dwNumTriangle = pMesh->GetNumTriangle();
				dwNumVertex = pMesh->GetNumVertex();
				dwVertexSize = pMesh->GetVertexSize();

				break;
			}
		case Base::Mesh_PMFMesh:
			{
				// ��ȡģ����Ϣ
				D3D9PMFMesh * pMesh = ( D3D9PMFMesh * ) pResource;
				pVertexBuffer = pMesh->GetVertexBuffer();
				pIndexBuffer = pMesh->GetIndexBuffer();
				pVertexDecl = pMesh->GetVertexDeclaration();
				dwNumTriangle = pMesh->GetNumTriangle();
				dwNumVertex = pMesh->GetNumVertex();
				dwVertexSize = pMesh->GetVertexSize();

				break;
			}
		case Base::Mesh_OgreMesh:
			{
				// ��ȡģ����Ϣ
				D3D9OgreMesh * pMesh = ( D3D9OgreMesh * ) pResource;
				pVertexBuffer = pMesh->GetVertexBuffer();
				pIndexBuffer = pMesh->GetIndexBuffer();
				pVertexDecl = pMesh->GetVertexDeclaration();
				dwNumTriangle = pMesh->GetNumTriangle();
				dwNumVertex = pMesh->GetNumVertex();
				dwVertexSize = pMesh->GetVertexSize();

				break;
			}
		case Base::Mesh_Terrain:
			{
				// ��ȡģ����Ϣ
				D3D9TerrainTile * pMesh = ( D3D9TerrainTile * ) pResource;
				pVertexBuffer = pMesh->GetVertexBuffer();
				pIndexBuffer = pMesh->GetIndexBuffer();
				pVertexDecl = pMesh->GetVertexDeclaration();
				dwNumTriangle = pMesh->GetNumTriangle();
				dwNumVertex = pMesh->GetNumVertex();
				dwVertexSize = pMesh->GetVertexSize();

				break;
			}
		default:
			break;
		}

		pD3D9Device->SetStreamSource( 0, pVertexBuffer, 0, dwVertexSize );
		pD3D9Device->SetIndices( pIndexBuffer );
		pD3D9Device->SetVertexDeclaration( pVertexDecl );

		for ( int i = 0; i < (int)uiPass; i++ )
		{
			if ( pMaterial->BeginPass( i ) )
			{
				pD3D9Device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, dwNumVertex, 0, dwNumTriangle );
				pMaterial->EndPass();
			}
		}

		// ��������
		pMaterial->End();

		return true;
	}

	bool    D3D9Renderer::RenderQuad( Effect * pEffect, float fLeftU, float fTopV, float fRightU, float fBottomV )
	{
		D3DSURFACE_DESC DescRT;
		LPDIRECT3DSURFACE9 pSurfRT;

		// ��ȡD3D9�豸ָ��
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// ��ȡ��Ⱦ���󳤿�
		pD3D9Device->GetRenderTarget( 0, &pSurfRT );
		pSurfRT->GetDesc( &DescRT );
		pSurfRT->Release();

		// λ��0.5
		float fWidth5 = (float) DescRT.Width - 0.5f;
		float fHeight5 = (float) DescRT.Height - 0.5f;

		// �����ı���
		ScreenVertex svQuad[4];

		svQuad[0].vPosition = D3DXVECTOR4( -0.5f, -0.5f, 0.5f, 1.0f );
		svQuad[0].vTextureCoord = D3DXVECTOR2( fLeftU, fTopV );

		svQuad[1].vPosition = D3DXVECTOR4( fWidth5, -0.5f, 0.5f, 1.0f );
		svQuad[1].vTextureCoord = D3DXVECTOR2( fRightU, fTopV );

		svQuad[2].vPosition = D3DXVECTOR4( -0.5f, fHeight5, 0.5f, 1.0f );
		svQuad[2].vTextureCoord = D3DXVECTOR2( fLeftU, fBottomV );

		svQuad[3].vPosition = D3DXVECTOR4( fWidth5, fHeight5, 0.5f, 1.0f );
		svQuad[3].vTextureCoord = D3DXVECTOR2( fRightU, fBottomV );

		UINT uiPassCount, uiPass;       
		if ( pEffect->Begin( &uiPassCount ) )
		{
			for ( uiPass = 0; uiPass < uiPassCount; uiPass++ )
			{
				pEffect->BeginPass(uiPass);
				pD3D9Device->SetRenderState( D3DRS_ZENABLE, FALSE );
				pD3D9Device->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
				pD3D9Device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, svQuad, sizeof(ScreenVertex) );
				pD3D9Device->SetRenderState( D3DRS_ZENABLE, TRUE );
				pEffect->EndPass();
			}

		    pEffect->End();
		}

		return true;
	}

	bool    D3D9Renderer::RenderText( int iX, int iY, Vector4f& vColor, wstring& szText )
	{
		// ���ò���
		RECT rc;
		SetRect( &rc, iX, iY, 0, 0 );
		D3DXCOLOR color( vColor[0], vColor[1], vColor[2], vColor[3] );

		// ��ȡ����
		LPD3DXFONT pFont = ( (D3D9RenderWindow *) RenderWindow::GetActiveRenderWindow() )->GetFont();

		// ��Ⱦ����
		HRESULT hr = pFont->DrawText( NULL, szText.c_str(), -1, &rc, DT_NOCLIP, color );

		return true;
	}
}