#include "D3D10PMFMesh.h"
#include "D3D10RenderWindow.h"
#include "../Main/BoundingBox.h"

namespace Flagship
{
	D3D10PMFMesh::D3D10PMFMesh()
	{
		m_iClassType = Base::Mesh_PMFMesh;
		m_pBound = new BoundingBox;
	}

	D3D10PMFMesh::~D3D10PMFMesh()
	{
		SAFE_DELETE( m_pBound );
	}

	bool    D3D10PMFMesh::Create()
	{
		if ( ! PMFMesh::Create() )
		{
			return false;
		}

    	m_iElementNum = 0;
		DWORD dwOffset = 0;
		#define ADD_DECL( offset, type, usage, index )  { D3D10_INPUT_ELEMENT_DESC tmp_decl = \
				{  usage \
				, (UINT)index \
				, type \
				, 0 \
				, offset \
				, D3D10_INPUT_PER_VERTEX_DATA \
				, 0 \
				}; \
				m_pInputElement[m_iElementNum++] = tmp_decl; }
		m_dwIndexFormat = DXGI_FORMAT_R16_UINT;

		if ( m_dwFVF & Mesh_Position )
		{
			ADD_DECL( dwOffset, DXGI_FORMAT_R32G32B32_FLOAT, "POSITION", 0);
			dwOffset += sizeof(Vector3f);
		}

		if ( m_dwFVF & Mesh_BlendWeights )
		{
			ADD_DECL( dwOffset, DXGI_FORMAT_R32G32B32_FLOAT, "BLENDWEIGHT", 0);
			dwOffset += sizeof(Vector3f);
		}

		if ( m_dwFVF & Mesh_BlendIndices )
		{
			ADD_DECL( dwOffset, DXGI_FORMAT_R16G16B16A16_UINT, "BLENDINDICES", 0);
			dwOffset += sizeof(WORD) * 4;
		}

		if ( m_dwFVF & Mesh_Normal )
		{
			ADD_DECL( dwOffset, DXGI_FORMAT_R32G32B32_FLOAT, "NORMAL", 0);
			dwOffset += sizeof(Vector3f);
		}

		if ( m_dwFVF & Mesh_Tangent )
		{
			ADD_DECL( dwOffset, DXGI_FORMAT_R32G32B32_FLOAT, "TANGENT", 0);
			dwOffset += sizeof(Vector3f);
		}

		if ( m_dwFVF & Mesh_Binormal )
		{
			ADD_DECL( dwOffset, DXGI_FORMAT_R32G32B32_FLOAT, "BINORMAL", 0);
			dwOffset += sizeof(Vector3f);
		}

		if ( m_dwFVF & Mesh_Texcoord1 )
		{
			ADD_DECL( dwOffset, DXGI_FORMAT_R32G32_FLOAT, "TEXCOORD", 0 );
			dwOffset += sizeof(Vector2f);
		}

		if ( m_dwFVF & Mesh_Texcoord2 )
		{
			ADD_DECL( dwOffset, DXGI_FORMAT_R32G32_FLOAT, "TEXCOORD", 1 );
			dwOffset += sizeof(Vector2f);
		}

		if ( m_dwFVF & Mesh_Texcoord3 )
		{
			ADD_DECL( dwOffset, DXGI_FORMAT_R32G32_FLOAT, "TEXCOORD", 2 );
			dwOffset += sizeof(Vector2f);
		}

		if ( m_dwFVF & Mesh_Texcoord4 )
		{
			ADD_DECL( dwOffset, DXGI_FORMAT_R32G32_FLOAT, "TEXCOORD", 3 );
			dwOffset += sizeof(Vector2f);
		}

		return true;
	}

	bool    D3D10PMFMesh::Cache()
	{
		if ( ! PMFMesh::Cache() )
		{
			return false;
		}

		// 获取D3D10设备指针
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 创建顶点缓存
		BYTE * pVertices = new BYTE[m_dwNumVertex * m_dwVertexSize];
		DWORD dwOffset = 0;

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

		D3D10_BUFFER_DESC VBDesc;
		VBDesc.ByteWidth = m_dwNumVertex * m_dwVertexSize;
		VBDesc.Usage = D3D10_USAGE_DEFAULT;
		VBDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		VBDesc.CPUAccessFlags = 0;
		VBDesc.MiscFlags = 0;

		D3D10_SUBRESOURCE_DATA InitVBData;
		InitVBData.pSysMem = pVertices;
		InitVBData.SysMemPitch = 0;
		InitVBData.SysMemSlicePitch = 0;

		pD3D10Device->CreateBuffer( &VBDesc, &InitVBData, &m_pD3D10VertexBuffer );
		delete [] pVertices;

		// 创建索引缓存
		BYTE * pIndecs = new BYTE[m_dwNumTrangle * 3 * sizeof(WORD)];
		memcpy( pIndecs, m_kPMFMeshData.pFaceIndices, m_dwNumTrangle * 3 * sizeof(WORD) );

		D3D10_BUFFER_DESC IBDesc;
		IBDesc.ByteWidth = m_dwNumTrangle * 3 * sizeof(WORD);
		IBDesc.Usage = D3D10_USAGE_DEFAULT;
		IBDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
		IBDesc.CPUAccessFlags = 0;
		IBDesc.MiscFlags = 0;

		D3D10_SUBRESOURCE_DATA InitIBData;
		InitIBData.pSysMem = pIndecs;
		InitIBData.SysMemPitch = 0;
		InitIBData.SysMemSlicePitch = 0;

		pD3D10Device->CreateBuffer( &IBDesc, &InitIBData, &m_pD3D10IndexBuffer );
		delete [] pIndecs;

		m_bReady = true;
		return true;
	}

	void    D3D10PMFMesh::UnCache()
	{
		PMFMesh::UnCache();
		SAFE_RELEASE( m_pD3D10VertexBuffer );
		SAFE_RELEASE( m_pD3D10IndexBuffer );
	}
}