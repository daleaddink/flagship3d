#include "PMFMesh.h"
#include "Buffer.h"
#include "BoundingBox.h"

#include <Common/LogManager.h>

namespace Flagship
{
	PMFMesh::PMFMesh()
	{
		m_iClassType = Base::Mesh_PMFMesh;
		m_pBound = new BoundingBox;
	}

	PMFMesh::~PMFMesh()
	{
		SAFE_DELETE( m_pBound );
	}

	bool    PMFMesh::CreateMesh( DWORD dwNumVerts, DWORD dwNumFaces, DWORD dwFVF, DWORD dwOptions )
	{
		return true;
	}

	bool     PMFMesh::Create()
	{
		if ( ! PMFParseInMemory( m_kPMFMeshData, (void *) m_kFileBuffer.GetPointer(), m_kFileBuffer.GetSize() ) )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			sprintf( szLog, "PMFMesh::PMFParseInMemory() Fail! File:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );
		}

		m_dwFVF = 0;
		m_dwNumTrangle = m_kPMFMeshData.NumFaces;
		m_dwNumVertex = m_kPMFMeshData.NumVertices;
		m_dwVertexSize = 0;
		
		if ( m_kPMFMeshData.pPositions != 0 )
		{
			m_dwFVF |= Mesh_Position;
			m_dwVertexSize += sizeof(Vector3f);
		}

		if ( m_kPMFMeshData.pBlendWeights != 0 )
		{
			m_dwFVF |= Mesh_BlendWeights;
			m_dwVertexSize += sizeof(Vector3f);
		}

		if ( m_kPMFMeshData.pBlendIndices != 0 )
		{
			m_dwFVF |= Mesh_BlendIndices;
			m_dwVertexSize += sizeof(WORD) * 4;
		}

		if ( m_kPMFMeshData.pNormals != 0 )
		{
			m_dwFVF |= Mesh_Normal;
			m_dwVertexSize += sizeof(Vector3f);
		}

		if ( m_kPMFMeshData.pTangents != 0 )
		{
			m_dwFVF |= Mesh_Tangent;
			m_dwVertexSize += sizeof(Vector3f);
		}

		if ( m_kPMFMeshData.pBinormals != 0 )
		{
			m_dwFVF |= Mesh_Binormal;
			m_dwVertexSize += sizeof(Vector3f);
		}

		if ( m_kPMFMeshData.pTC[0] != 0 )
		{
			m_dwFVF |= Mesh_Texcoord1;
			m_dwVertexSize += sizeof(Vector2f);
		}

		if ( m_kPMFMeshData.pTC[1] != 0 )
		{
			m_dwFVF |= Mesh_Texcoord2;
			m_dwVertexSize += sizeof(Vector2f);
		}

		if ( m_kPMFMeshData.pTC[2] != 0 )
		{
			m_dwFVF |= Mesh_Texcoord3;
			m_dwVertexSize += sizeof(Vector2f);
		}

		if ( m_kPMFMeshData.pTC[3] != 0 )
		{
			m_dwFVF |= Mesh_Texcoord4;
			m_dwVertexSize += sizeof(Vector2f);
		}

		return true;
	}

	void    PMFMesh::Destory()
	{
		m_dwFVF = 0;
		m_dwNumTrangle = 0;
		m_dwNumVertex = 0;
		m_dwVertexSize = 0;
	}

	bool    PMFMesh::PMFParseInMemory( PMFMeshData& meshData, void * pFileData, DWORD DataLength )
	{
		//-------------------------------------------------------------//
		const DWORD	PMF_MAGIC = PG_MAKEFOURCC('P', 'M', 'F', '\0');
		const DWORD	PMF_VERSION = 200;
		//-------------------------------------------------------------//

		PMF_MESH_HEAD FileHead;

		char* p = (char*)pFileData;

		// Read file head size
		DWORD FileHeadSize = *( (DWORD*)p );	
		FileHeadSize = __min(FileHeadSize, sizeof(FileHead));
		p += sizeof(DWORD);

		// Read file head	
		memset(&FileHead, 0, sizeof(FileHead) );
		memcpy(&FileHead, p, FileHeadSize );
		p += FileHeadSize;

		if ( FileHead.Magic != PMF_MAGIC || FileHead.Version != PMF_VERSION )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			sprintf( szLog, "OgreMesh::Create Version Error! File:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		DWORD NumVertices = FileHead.NumVertices;
		meshData.NumVertices = FileHead.NumVertices;
		meshData.NumFaces	 = FileHead.NumFaces;
		meshData.NumTCs		 = FileHead.NumTCs;
		meshData.AttrTableSize = FileHead.AttrTableSize;

		// Read attribute table
		if ( meshData.AttrTableSize > 0 )
		{
			meshData.pAttrTable = (MESH_ATTRIBUTE_RANGE*)p;
			p += sizeof(MESH_ATTRIBUTE_RANGE) * meshData.AttrTableSize;
		}

		if ( FileHead.bPosition )
		{
			meshData.pPositions = (Vector3f*)p;
			p += sizeof(Vector3f) * NumVertices;
		}

		if ( FileHead.bBlendWeight )
		{
			meshData.pBlendWeights = (Vector3f*)p;
			p += sizeof(Vector3f) * NumVertices;
		}

		if ( FileHead.bBlendIndex )
		{
			meshData.pBlendIndices = (WORD*)p;
			p += sizeof(WORD) * NumVertices * 4;
		}

		if ( FileHead.bNormal )
		{
			meshData.pNormals = (Vector3f*)p;
			p += sizeof(Vector3f) * NumVertices;
		}

		if ( FileHead.bTangent  )
		{
			meshData.pTangents = (Vector3f*)p;
			p += sizeof(Vector3f) * NumVertices;

			meshData.pBinormals = (Vector3f*)p;
			p += sizeof(Vector3f) * NumVertices;
		}

		if ( FileHead.bWeight )
		{
			meshData.pWeights = (float*)p;
			p += sizeof(float) * NumVertices;
		}

		if ( FileHead.bDiffuses )
		{
			meshData.pDiffuses = (DWORD*)p;
			p += sizeof(DWORD) * NumVertices;
		}

		if ( FileHead.bSpecular )
		{
			meshData.pSpeculars = (DWORD*)p;
			p += sizeof(DWORD) * NumVertices;
		}

		if ( FileHead.NumTCs > 0 )
		{
			meshData.pTC[0] = (Vector2f*)p;
			p += sizeof(Vector2f) * NumVertices;
		}
		if ( FileHead.NumTCs > 1  )
		{
			meshData.pTC[1] = (Vector2f*)p;
			p += sizeof(Vector2f) * NumVertices;
		}
		if ( FileHead.NumTCs > 2  )
		{
			meshData.pTC[2] = (Vector2f*)p;
			p += sizeof(Vector2f) * NumVertices;
		}
		if ( FileHead.NumTCs > 3  ) 
		{
			meshData.pTC[3] = (Vector2f*)p;
			p += sizeof(Vector2f) * NumVertices;
		}

		// Read face indices
		meshData.pFaceIndices = (WORD*)p;
		p += sizeof(WORD) * 3 * meshData.NumFaces;

		return true;
	}
}