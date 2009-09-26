#include "D3D10SDKMesh.h"
#include "D3D10RenderWindow.h"

namespace Flagship
{
	D3D10SDKMesh::D3D10SDKMesh()
	{
		m_iClassType = Base::Mesh_SDKMesh;
		m_pBound = new BoundingBox;

		m_pSDKMesh = new CDXUTSDKMesh;
	}

	D3D10SDKMesh::~D3D10SDKMesh()
	{
		SAFE_DELETE( m_pBound );
		SAFE_DELETE( m_pSDKMesh );

		m_pD3D10VertexBuffer = NULL;
		m_pD3D10IndexBuffer = NULL;
	}

	bool    D3D10SDKMesh::Create()
	{
		// 获取D3D10设备指针
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 创建DXUT对象
		m_pSDKMesh->Create( pD3D10Device, m_szPathName.c_str() );

		const D3D10_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		};
		
		for ( int i = 0; i < 3; i++ )
		{
			m_pInputElement[i] = layout[i];
		}
		m_iElementNum = 3;

		m_dwIndexFormat = m_pSDKMesh->GetIBFormat10( 0 );

		return true;
	}

	void    D3D10SDKMesh::Destory()
	{
		Mesh::Destory();

		// 释放DXUT对象
		m_pSDKMesh->Destroy();
	}

	bool    D3D10SDKMesh::Cache()
	{
		if ( ! Mesh::Cache() )
		{
			return false;
		}

		// 获取缓冲
		m_pD3D10VertexBuffer = m_pSDKMesh->GetVB10( 0, 0 );
		m_pD3D10IndexBuffer = m_pSDKMesh->GetIB10( 0 );

		// 获取缓冲信息
		m_dwNumTrangle = (DWORD) m_pSDKMesh->GetNumIndices( 0 ) / 3;
		m_dwNumVertex = (DWORD) m_pSDKMesh->GetNumVertices( 0, 0 );
		m_dwVertexSize = m_pSDKMesh->GetVertexStride( 0, 0 );

		// 设置标记
		m_bReady = true;
		return true;
	}

	void    D3D10SDKMesh::UnCache()
	{
		Mesh::UnCache();
		SAFE_RELEASE( m_pD3D10VertexBuffer );
		SAFE_RELEASE( m_pD3D10IndexBuffer );
	}
}