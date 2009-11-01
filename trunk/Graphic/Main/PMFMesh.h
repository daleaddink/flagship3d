#ifndef _PMFMESH_H_
#define _PMFMESH_H_

#include <Common/Prerequisites.h>
#include "Mesh.h"

namespace Flagship
{
    #pragma pack(push, 4)

	struct PMF_MESH_HEAD
	{
		DWORD Magic;
		DWORD Version;
		DWORD bPosition		: 1;
		DWORD bBlendWeight	: 1;
		DWORD bBlendIndex	: 1;	
		DWORD bWeight		: 1;	
		DWORD bNormal		: 1;	
		DWORD bTangent		: 1;
		DWORD bDiffuses		: 1;
		DWORD bSpecular		: 1;
		DWORD bUnused		: 24;
		DWORD NumVertices;
		DWORD NumFaces;
		DWORD NumTCs;
		DWORD AttrTableSize;
	};

	struct MESH_ATTRIBUTE_RANGE
	{
		DWORD FaceStart;
		DWORD FaceCount;
		DWORD VertexStart;
		DWORD VertexCount;
	};

	struct PMFMeshData
	{
		PMFMeshData()
		{
			pAttrTable		 = NULL;
			pPositions		 = NULL;
			pBlendWeights	 = NULL;
			pBlendIndices	 = NULL;
			pWeights		 = NULL;
			pNormals		 = NULL;
			pTangents		 = NULL;
			pDiffuses		 = NULL;
			pSpeculars		 = NULL;
			pTC[0]			 = NULL;
			pTC[1]			 = NULL;
			pTC[2]			 = NULL;
			pTC[3]			 = NULL;
			pFaceIndices	 = NULL;
			pBinormals		 = NULL;
			NumFaces		 = 0;
			NumVertices		 = 0;
			AttrTableSize	 = 0;
			NumTCs			 = 0;
		}

		DWORD				 AttrTableSize;	
		DWORD				 NumFaces;
		DWORD				 NumVertices;
		DWORD				 NumTCs;

		MESH_ATTRIBUTE_RANGE * pAttrTable;
		Vector3f *			 pPositions;
		Vector3f *			 pBlendWeights;
		WORD *				 pBlendIndices;
		float*				 pWeights;
		Vector3f *			 pNormals;
		Vector3f *			 pTangents;
		Vector3f *			 pBinormals;
		DWORD *				 pDiffuses;
		DWORD *				 pSpeculars;
		Vector2f *		 	 pTC[4];
		WORD *				 pFaceIndices;
	};

    #pragma pack(pop)

	#define PG_MAKEFOURCC(ch0, ch1, ch2, ch3)                               \
		((DWORD)(char)(ch0) | ((DWORD)(char)(ch1) << 8) |				\
		((DWORD)(char)(ch2) << 16) | ((DWORD)(char)(ch3) << 24 ))

	class Buffer;

	class _DLL_Export PMFMesh : public Mesh
	{
	public:
		PMFMesh();
		virtual ~PMFMesh();

		// 创建模型
		bool                  CreateMesh( DWORD dwNumVerts, DWORD dwNumFaces, DWORD dwFVF, DWORD dwOptions );

	protected:
		// 读取资源
		virtual bool          Create();

		// 释放资源
		virtual void          Destory();

		// 获取顶点数据
		virtual Vector3f *    GetVertexData() { return m_kPMFMeshData.pPositions; }

		// 获取索引数据
		virtual WORD *        GetIndexData() { return m_kPMFMeshData.pFaceIndices; }

		// 获取法线数据
		virtual Vector3f *    GetNormalData() { return m_kPMFMeshData.pNormals; }

	protected:
		// 分析PMF数据
		bool                  PMFParseInMemory( PMFMeshData& meshData, void* pFileData, DWORD DataLength );
		
	protected:
		// PMF结构
		PMFMeshData           m_kPMFMeshData;
	};
}

#endif