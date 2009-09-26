#ifndef _MESH_H_
#define _MESH_H_

#include <Common/Prerequisites.h>
#include "Resource.h"

namespace Flagship
{
	class BoundingVolume;

	class _DLL_Export Mesh : public Resource
	{
	public:
		Mesh();
		virtual ~Mesh();

		enum
		{
			Mesh_Position      = 1,
			Mesh_BlendWeights   = 2,
			Mesh_BlendIndices  = 4,
			Mesh_Normal        = 8,
			Mesh_Tangent       = 16,
			Mesh_Binormal      = 32,
			Mesh_Texcoord1     = 64,
			Mesh_Texcoord2     = 128,
			Mesh_Texcoord3     = 256,
			Mesh_Texcoord4     = 512,
		};

		// 获取三角形数
		DWORD                 GetNumTriangle() { return m_dwNumTrangle; }

		// 获取顶点数
		DWORD                 GetNumVertex() { return m_dwNumVertex; }

		// 获取顶点大小
		DWORD                 GetVertexSize() { return m_dwVertexSize; }

	public:
	    // 获取顶点数据
		virtual Vector3f *    GetVertexData() { return NULL; }

		// 获取索引数据
		virtual WORD *        GetIndexData() { return NULL; }

		// 获取法线数据
		virtual Vector3f *    GetNormalData() { return NULL; }

	protected:
		// 顶点格式
		DWORD                 m_dwFVF;

		// 三角形数
		DWORD                 m_dwNumTrangle;

		// 顶点数
		DWORD                 m_dwNumVertex;

		// 顶点大小
		DWORD                 m_dwVertexSize;

		// 包围体
		BoundingVolume *      m_pBound;

	private:
	};
}

#endif