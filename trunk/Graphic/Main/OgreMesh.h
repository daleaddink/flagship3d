#ifndef _OGREMESH_H_
#define _OGREMESH_H_

#include <Common/Prerequisites.h>
#include "Mesh.h"

namespace Flagship
{
	class _DLL_Export OgreMesh : public Mesh
	{
	public:
		OgreMesh();
		virtual ~OgreMesh();

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
		
	private:
		
	};
}

#endif