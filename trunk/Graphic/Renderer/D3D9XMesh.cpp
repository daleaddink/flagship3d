#include "D3D9XMesh.h"
#include "D3D9RenderWindow.h"
#include "../Main/Texture.h"
#include "../Main/BoundingBox.h"

namespace Flagship
{
	D3D9XMesh::D3D9XMesh()
	{
		m_iClassType = Base::Mesh_XMesh;
		m_pD3D9Mesh = NULL;
		m_pBound = new BoundingBox;
	}

	D3D9XMesh::~D3D9XMesh()
	{
		SAFE_RELEASE( m_pD3D9Mesh );
		SAFE_DELETE( m_pBound );
	}

	bool    D3D9XMesh::ComputeMeshInfo()
	{
		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		//====================================================================//
		// Check if the old declaration contains normals, tangents, binormals //
		//====================================================================//
		bool bHadNormal   = false;
		bool bHadTangent  = false;
		bool bHadBinormal = false;

		D3DVERTEXELEMENT9 vertexOldDecl[ MAX_FVF_DECL_SIZE ];

		if ( m_pD3D9Mesh && SUCCEEDED( m_pD3D9Mesh->GetDeclaration( vertexOldDecl ) ) )
		{
			// Go through the declaration and look for the right channels, hoping for a match:
			for( UINT iChannelIndex = 0; iChannelIndex < D3DXGetDeclLength( vertexOldDecl ); iChannelIndex++ )
			{
				if( vertexOldDecl[iChannelIndex].Usage == D3DDECLUSAGE_NORMAL )
				{
					bHadNormal = true;
				}

				if( vertexOldDecl[iChannelIndex].Usage == D3DDECLUSAGE_TANGENT )
				{
					bHadTangent = true;
				}

				if( vertexOldDecl[iChannelIndex].Usage == D3DDECLUSAGE_BINORMAL )
				{
					bHadBinormal = true;
				}
			}  
		}

		if ( bHadNormal && bHadTangent && bHadBinormal )
		{
			return true;
		}

		const D3DVERTEXELEMENT9 vertexDecl[] =
		{
			{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
			{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0 },
			{ 0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
			D3DDECL_END()
		};

		LPD3DXMESH pTempMesh = NULL;

		// Clone mesh to match the specified declaration: 
		if ( FAILED( m_pD3D9Mesh->CloneMesh( m_pD3D9Mesh->GetOptions(), vertexDecl, pD3D9Device, &pTempMesh ) ))
		{
			SAFE_RELEASE( pTempMesh );
			return false;
		}

		if ( pTempMesh == NULL )
		{
			// We failed to clone the mesh and we need the tangent space for our effect:
			return false;
		}

		//==============================================================//
		// Generate normals / tangents / binormals if they were missing //
		//==============================================================//
		SAFE_RELEASE( m_pD3D9Mesh );
		m_pD3D9Mesh = pTempMesh;

		if( ! bHadNormal )
		{
			// Compute normals in case the meshes have them
			D3DXComputeNormals( m_pD3D9Mesh, NULL );
		}  

		DWORD *rgdwAdjacency = NULL;
		rgdwAdjacency = new DWORD[ m_pD3D9Mesh->GetNumFaces() * 3 ];

		if( rgdwAdjacency == NULL )
		{
			return false;
		}
		m_pD3D9Mesh->GenerateAdjacency( 1e-6f, rgdwAdjacency );

		// Optimize the mesh for this graphics card's vertex cache 
		// so when rendering the mesh's triangle list the vertices will 
		// cache hit more often so it won't have to re-execute the vertex shader 
		// on those vertices so it will improve perf.     
		m_pD3D9Mesh->OptimizeInplace( D3DXMESHOPT_VERTEXCACHE, rgdwAdjacency, NULL, NULL, NULL );

		if ( ! bHadTangent || ! bHadBinormal )
		{
			ID3DXMesh* pNewMesh;

			// Compute tangents, which are required for normal mapping
			if ( FAILED( D3DXComputeTangentFrameEx( m_pD3D9Mesh, D3DDECLUSAGE_TEXCOORD, 0, D3DDECLUSAGE_TANGENT, 0, D3DDECLUSAGE_BINORMAL, 0, 
				D3DDECLUSAGE_NORMAL, 0, 0, rgdwAdjacency, -1.01f,
				-0.01f, -1.01f, &pNewMesh, NULL ) ) )
			{
				return false;
			}

			SAFE_RELEASE( m_pD3D9Mesh );
			m_pD3D9Mesh = pNewMesh;
		}  

		SAFE_DELETE_ARRAY( rgdwAdjacency );
		return true;
	}

	bool    D3D9XMesh::Create()
	{
		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 创建D3D9模型对象
		HRESULT hr = D3DXLoadMeshFromX( m_szPathName.c_str(), D3DXMESH_MANAGED, pD3D9Device, NULL, NULL, NULL, NULL, &m_pD3D9Mesh );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			sprintf( szLog, "D3D9XMesh::Load() Fail! File:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		// 获取模型格式描述
		D3DVERTEXELEMENT9 VertexDecls[MAX_FVF_DECL_SIZE];
		m_pD3D9Mesh->GetDeclaration( VertexDecls );
		hr = pD3D9Device->CreateVertexDeclaration( VertexDecls, &m_pD3D9VertexDecl );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			sprintf( szLog, "D3D9XMesh::Load():CreateVertexDeclaration() Fail! File:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

//		ComputeMeshInfo();
//		D3DXSaveMeshToX( L"G:\\Test.x", m_pD3D9Mesh, NULL, NULL, NULL, 0, D3DXF_FILEFORMAT_TEXT );
		
		return true;
	}

	void    D3D9XMesh::Destory()
	{
		SAFE_RELEASE( m_pD3D9VertexDecl );
		SAFE_RELEASE( m_pD3D9Mesh );
	}

	bool    D3D9XMesh::Cache()
	{
		if ( ! Mesh::Cache() )
		{
			return false;
		}

		// 获取缓冲
		m_pD3D9Mesh->GetVertexBuffer( &m_pD3D9VertexBuffer );
		m_pD3D9Mesh->GetIndexBuffer( &m_pD3D9IndexBuffer );
		
		// 获取缓冲信息
		m_dwNumTrangle = m_pD3D9Mesh->GetNumFaces();
		m_dwNumVertex = m_pD3D9Mesh->GetNumVertices();
		m_dwVertexSize = m_pD3D9Mesh->GetNumBytesPerVertex();

		// 设置标记
		m_bReady = true;
		return true;
	}

	void    D3D9XMesh::UnCache()
	{
		Mesh::UnCache();
		SAFE_RELEASE( m_pD3D9VertexBuffer );
		SAFE_RELEASE( m_pD3D9IndexBuffer );
	}
}