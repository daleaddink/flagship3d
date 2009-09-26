#include "TerrainTile.h"

namespace Flagship
{
	TerrainTile::TerrainTile()
	{
		m_iClassType = Base::Mesh_Terrain;

		float afTemp[3] = { FLT_MAX, FLT_MAX, FLT_MAX };
		m_pBound = new BoundingBox( Vector3f( 0.0f, 0.0f, 0.0f ), NULL, afTemp );
	}

	TerrainTile::~TerrainTile()
	{
		SAFE_DELETE( m_pBound );
		SAFE_DELETE_ARRAY( m_pVertexList );
		SAFE_DELETE_ARRAY( m_pIndexList );
	}

	void    TerrainTile::SetConfig( int iSizeX, int iSizeZ, int iLod )
	{
		m_iSizeX = iSizeX;
		m_iSizeZ = iSizeZ;
		m_iLod = iLod;
	}

	bool    TerrainTile::Create()
	{
		m_dwNumTrangle = m_iSizeX * m_iSizeZ * 2;
		m_dwNumVertex = ( m_iSizeX + 1 ) * ( m_iSizeZ + 1 );
		m_dwVertexSize = sizeof(Vector3f);

		m_pVertexList = new Vector3f[m_dwNumVertex];
		m_pIndexList = new DWORD[m_dwNumTrangle * 3];

		for ( int i = 0; i <= m_iSizeX; i++ )
		{
			for ( int j = 0; j <= m_iSizeZ; j++ )
			{
				m_pVertexList[i * ( m_iSizeX + 1 ) + j] = Vector3f( (float)( i - m_iSizeX / 2 ), 0.0f, (float) ( j - m_iSizeZ / 2 ) );
			}
		}

		int iCount = 0;
		for ( int i = 0; i < m_iSizeX; i++ )
		{
			for ( int j = 0; j < m_iSizeZ; j++ )
			{
				int iBase = i * ( m_iSizeX + 1 ) + j;
				m_pIndexList[iCount] = iBase;
				m_pIndexList[iCount + 1] = iBase + 1;
				m_pIndexList[iCount + 2] = iBase + m_iSizeX + 1;
				m_pIndexList[iCount + 3] = iBase + 1;
				m_pIndexList[iCount + 4] = iBase + m_iSizeZ + 2;
				m_pIndexList[iCount + 5] = iBase + m_iSizeZ + 1;
				iCount += 6;
			}
		}

		m_dwFVF = Mesh_Position;
		return true;
	}

	void    TerrainTile::Destory()
	{
		m_dwFVF = 0;
		m_dwNumTrangle = 0;
		m_dwNumVertex = 0;
		m_dwVertexSize = 0;

		SAFE_DELETE_ARRAY( m_pVertexList );
		SAFE_DELETE_ARRAY( m_pIndexList );
	}
}