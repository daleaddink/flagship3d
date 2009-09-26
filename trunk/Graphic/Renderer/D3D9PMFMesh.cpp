#include "D3D9PMFMesh.h"
#include "D3D9RenderWindow.h"
#include "../Main/BoundingBox.h"

namespace Flagship
{
	D3D9PMFMesh::D3D9PMFMesh()
	{
		m_iClassType = Base::Mesh_PMFMesh;
		m_pBound = new BoundingBox;
	}

	D3D9PMFMesh::~D3D9PMFMesh()
	{
		SAFE_DELETE( m_pBound );
	}

	bool    D3D9PMFMesh::Create()
	{
		if ( ! PMFMesh::Create() )
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

	void    D3D9PMFMesh::Destory()
	{
		SAFE_RELEASE( m_pD3D9VertexDecl );
	}

	bool    D3D9PMFMesh::Cache()
	{
		if ( ! PMFMesh::Cache() )
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

		if ( m_dwFVF & Mesh_Position )
		{
			for ( int i = 0; i < (int) m_dwNumVertex; i++ )
			{
				Vector3f * pPosition = (Vector3f *)( pVertices + i * m_dwVertexSize );
				*pPosition = m_kPMFMeshData.pPositions[i];
			}

			dwOffset += sizeof(Vector3f);
		}

		if ( m_dwFVF & Mesh_BlendWeights )
		{
			for ( int i = 0; i < (int) m_dwNumVertex; i++ )
			{
				Vector3f * pBlendWeight = (Vector3f *)( pVertices + i * m_dwVertexSize + dwOffset );
				*pBlendWeight = m_kPMFMeshData.pBlendWeights[i];
			}

			dwOffset += sizeof(Vector3f);
		}

		if ( m_dwFVF & Mesh_BlendIndices )
		{
			for ( int i = 0; i < (int) m_dwNumVertex; i++ )
			{
				WORD * pBlendIndices = (WORD *)( pVertices + i * m_dwVertexSize + dwOffset );
				pBlendIndices[0] = m_kPMFMeshData.pBlendIndices[i * 4];
				pBlendIndices[1] = m_kPMFMeshData.pBlendIndices[i * 4 + 1];
				pBlendIndices[2] = m_kPMFMeshData.pBlendIndices[i * 4 + 2];
				pBlendIndices[3] = m_kPMFMeshData.pBlendIndices[i * 4 + 3];
			}

			dwOffset += sizeof(WORD) * 4;
		}

		if ( m_dwFVF & Mesh_Normal )
		{
			for ( int i = 0; i < (int) m_dwNumVertex; i++ )
			{
				Vector3f * pNormal = (Vector3f *)( pVertices + i * m_dwVertexSize + dwOffset );
				*pNormal = m_kPMFMeshData.pNormals[i];
			}

			dwOffset += sizeof(Vector3f);
		}

		if ( m_dwFVF & Mesh_Tangent )
		{
			for ( int i = 0; i < (int) m_dwNumVertex; i++ )
			{
				Vector3f * pTangent = (Vector3f *)( pVertices + i * m_dwVertexSize + dwOffset );
				*pTangent = m_kPMFMeshData.pTangents[i];
			}

			dwOffset += sizeof(Vector3f);
		}

		if ( m_dwFVF & Mesh_Binormal )
		{
			for ( int i = 0; i < (int) m_dwNumVertex; i++ )
			{
				Vector3f * pBinormal = (Vector3f *)( pVertices + i * m_dwVertexSize + dwOffset );
				*pBinormal = m_kPMFMeshData.pBinormals[i];
			}

			dwOffset += sizeof(Vector3f);
		}

		if ( m_dwFVF & Mesh_Texcoord1 )
		{
			for ( int i = 0; i < (int) m_dwNumVertex; i++ )
			{
				Vector2f * pTexcoord1 = (Vector2f *)( pVertices + i * m_dwVertexSize + dwOffset );
				*pTexcoord1 = m_kPMFMeshData.pTC[0][i];
			}

			dwOffset += sizeof(Vector2f);
		}

		if ( m_dwFVF & Mesh_Texcoord2 )
		{
			for ( int i = 0; i < (int) m_dwNumVertex; i++ )
			{
				Vector2f * pTexcoord2 = (Vector2f *)( pVertices + i * m_dwVertexSize + dwOffset );
				*pTexcoord2 = m_kPMFMeshData.pTC[1][i];
			}

			dwOffset += sizeof(Vector2f);
		}

		if ( m_dwFVF & Mesh_Texcoord3 )
		{
			for ( int i = 0; i < (int) m_dwNumVertex; i++ )
			{
				Vector2f * pTexcoord3 = (Vector2f *)( pVertices + i * m_dwVertexSize + dwOffset );
				*pTexcoord3 = m_kPMFMeshData.pTC[2][i];
			}

			dwOffset += sizeof(Vector2f);
		}

		if ( m_dwFVF & Mesh_Texcoord4 )
		{
			for ( int i = 0; i < (int) m_dwNumVertex; i++ )
			{
				Vector2f * pTexcoord4 = (Vector2f *)( pVertices + i * m_dwVertexSize + dwOffset );
				*pTexcoord4 = m_kPMFMeshData.pTC[3][i];
			}

			dwOffset += sizeof(Vector2f);
		}

		m_pD3D9VertexBuffer->Unlock();

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

		memcpy( pIndecs, m_kPMFMeshData.pFaceIndices, m_dwNumTrangle * 3 * sizeof(WORD) );

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