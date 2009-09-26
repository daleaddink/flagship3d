#include "D3D9TerrainTile.h"
#include "D3D9RenderWindow.h"

namespace Flagship
{
	D3D9TerrainTile::D3D9TerrainTile()
	{
		m_iClassType = Base::Mesh_Terrain;

		float afTemp[3] = { FLT_MAX, FLT_MAX, FLT_MAX };
		m_pBound = new BoundingBox( Vector3f( 0.0f, 0.0f, 0.0f ), NULL, afTemp );
	}

	D3D9TerrainTile::~D3D9TerrainTile()
	{
		SAFE_DELETE( m_pBound );
	}

	bool    D3D9TerrainTile::Create()
	{
		if ( ! TerrainTile::Create() )
		{
			return false;
		}

		D3DVERTEXELEMENT9 VertexDecls[MAX_FVF_DECL_SIZE];
		D3DVERTEXELEMENT9 TempDecl = { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 };
		VertexDecls[0] = TempDecl;
		D3DVERTEXELEMENT9 DeclEnd = { 0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0 };
		VertexDecls[1] = DeclEnd;

		// ��ȡD3D9�豸ָ��
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// ��ȡģ�͸�ʽ����
		HRESULT hr = pD3D9Device->CreateVertexDeclaration( VertexDecls, &m_pD3D9VertexDecl );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D9TerrainTile::Load():CreateVertexDeclaration() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}		

		return true;
	}

	void    D3D9TerrainTile::Destory()
	{
		SAFE_RELEASE( m_pD3D9VertexDecl );
	}

	bool    D3D9TerrainTile::Cache()
	{
		if ( ! TerrainTile::Cache() )
		{
			return false;
		}

		// ��ȡD3D9�豸ָ��
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// �������㻺��
		pD3D9Device->CreateVertexBuffer( m_dwVertexSize * m_dwNumVertex, 0, 0, D3DPOOL_MANAGED, &m_pD3D9VertexBuffer, NULL );

		// ������������
		BYTE * pVertices;
		DWORD dwOffset = 0;

		HRESULT hr = m_pD3D9VertexBuffer->Lock( 0, 0, (void **) &pVertices, 0 );
		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D9TerrainTile::Cache() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		for ( int i = 0; i < (int) m_dwNumVertex; i++ )
		{
			Vector3f * pPosition = (Vector3f *)( pVertices + i * m_dwVertexSize );
			*pPosition = m_pVertexList[i];
		}

		m_pD3D9VertexBuffer->Unlock();

		// ������������
		pD3D9Device->CreateIndexBuffer( m_dwNumTrangle * 3 * sizeof(DWORD), 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pD3D9IndexBuffer, NULL );

		// ������������
		BYTE * pIndecs;
		hr = m_pD3D9IndexBuffer->Lock( 0, 0, (void **) &pIndecs, 0 );
		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D9TerrainTile::Cache() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		memcpy( pIndecs, m_pIndexList, m_dwNumTrangle * 3 * sizeof(DWORD) );

		m_pD3D9IndexBuffer->Unlock();

		m_bReady = true;
		return true;
	}

	void    D3D9TerrainTile::UnCache()
	{
		TerrainTile::UnCache();
		SAFE_RELEASE( m_pD3D9VertexBuffer );
		SAFE_RELEASE( m_pD3D9IndexBuffer );
	}
}