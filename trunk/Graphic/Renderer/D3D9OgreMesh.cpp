#include "D3D9OgreMesh.h"
#include "D3D9RenderWindow.h"
#include "../Main/BoundingBox.h"

namespace Flagship
{
	D3D9OgreMesh::D3D9OgreMesh()
	{
		m_iClassType = Base::Mesh_OgreMesh;
		m_pBound = new BoundingBox;
	}

	D3D9OgreMesh::~D3D9OgreMesh()
	{
		SAFE_DELETE( m_pBound );
	}

	bool    D3D9OgreMesh::Create()
	{
		if ( ! OgreMesh::Create() )
		{
			return false;
		}

		D3DVERTEXELEMENT9 VertexDecls[MAX_FVF_DECL_SIZE];
		DWORD NumElements = 0;
		DWORD dwOffset = 0;
#define ADD_DECL( offset, type, usage, index )  { D3DVERTEXELEMENT9 tmp_decl = \
		{  0 \
		, (WORD)offset \
		, D3DDECLTYPE_##type \
		, D3DDECLMETHOD_DEFAULT \
		, D3DDECLUSAGE_##usage \
		, (BYTE)index \
		}; \
		VertexDecls[NumElements++] = tmp_decl; }
#define DECL_END() {0xFF,0,D3DDECLTYPE_UNUSED,0,0,0}

		if ( m_dwFVF & Mesh_Position )
		{
			ADD_DECL( dwOffset, FLOAT3, POSITION, 0);
			dwOffset += sizeof(Vector3f);
		}

		if ( m_dwFVF & Mesh_BlendWeights )
		{
			ADD_DECL( dwOffset, FLOAT3, BLENDWEIGHT, 0);
			dwOffset += sizeof(Vector3f);
		}

		if ( m_dwFVF & Mesh_BlendIndices )
		{
			ADD_DECL( dwOffset, D3DCOLOR, BLENDINDICES, 0);
			dwOffset += sizeof(WORD) * 4;
		}

		if ( m_dwFVF & Mesh_Normal )
		{
			ADD_DECL( dwOffset, FLOAT3, NORMAL, 0);
			dwOffset += sizeof(Vector3f);
		}

		if ( m_dwFVF & Mesh_Tangent )
		{
			ADD_DECL( dwOffset, FLOAT3, TANGENT, 0);
			dwOffset += sizeof(Vector3f);
		}

		if ( m_dwFVF & Mesh_Binormal )
		{
			ADD_DECL( dwOffset, FLOAT3, BINORMAL, 0);
			dwOffset += sizeof(Vector3f);
		}

		if ( m_dwFVF & Mesh_Texcoord1 )
		{
			ADD_DECL( dwOffset, FLOAT2, TEXCOORD, 0 );
			dwOffset += sizeof(Vector2f);
		}

		if ( m_dwFVF & Mesh_Texcoord2 )
		{
			ADD_DECL( dwOffset, FLOAT2, TEXCOORD, 1 );
			dwOffset += sizeof(Vector2f);
		}

		if ( m_dwFVF & Mesh_Texcoord3 )
		{
			ADD_DECL( dwOffset, FLOAT2, TEXCOORD, 2 );
			dwOffset += sizeof(Vector2f);
		}

		if ( m_dwFVF & Mesh_Texcoord4 )
		{
			ADD_DECL( dwOffset, FLOAT2, TEXCOORD, 3 );
			dwOffset += sizeof(Vector2f);
		}

		D3DVERTEXELEMENT9 decl_end = DECL_END();
		VertexDecls[NumElements++] = decl_end;

		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 获取模型格式描述
		HRESULT hr = pD3D9Device->CreateVertexDeclaration( VertexDecls, &m_pD3D9VertexDecl );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			sprintf( szLog, "D3D9PMFMesh::Load():CreateVertexDeclaration() Fail! File:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	void    D3D9OgreMesh::Destory()
	{
		SAFE_RELEASE( m_pD3D9VertexDecl );
	}

	bool    D3D9OgreMesh::Cache()
	{
		if ( ! OgreMesh::Cache() )
		{
			return false;
		}

		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 创建顶点缓存
		pD3D9Device->CreateVertexBuffer( m_dwVertexSize * m_dwNumVertex, 0, 0, D3DPOOL_MANAGED, &m_pD3D9VertexBuffer, NULL );

		// 拷贝顶点数据
		BYTE * pVertices;
		DWORD dwOffset = 0;

		HRESULT hr = m_pD3D9VertexBuffer->Lock( 0, 0, (void **) &pVertices, 0 );
		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			sprintf( szLog, "D3D9PMFMesh::Cache() Fail! File:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		memcpy( pVertices, m_pVertexData, m_dwVertexSize * m_dwNumVertex );

		m_pD3D9VertexBuffer->Unlock();

		int iIndexType = D3DFMT_INDEX16;
		if ( m_b32Index )
		{
			iIndexType = D3DFMT_INDEX32;
		}

		// 创建索引缓存
		pD3D9Device->CreateIndexBuffer( m_dwNumTrangle * 3 * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pD3D9IndexBuffer, NULL );

		// 拷贝顶点数据
		BYTE * pIndecs;
		hr = m_pD3D9IndexBuffer->Lock( 0, 0, (void **) &pIndecs, 0 );
		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			sprintf( szLog, "D3D9PMFMesh::Cache() Fail! File:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		int iIndexSize = sizeof(WORD);
		if ( m_b32Index )
		{
			iIndexSize = sizeof(int);
		}
		memcpy( pIndecs, m_pIndexData, m_dwNumTrangle * 3 * iIndexSize );

		m_pD3D9IndexBuffer->Unlock();

		m_bReady = true;
		return true;
	}

	void    D3D9PMFMesh::UnCache()
	{
		PMFMesh::UnCache();
		SAFE_RELEASE( m_pD3D9VertexBuffer );
		SAFE_RELEASE( m_pD3D9IndexBuffer );
	}
}