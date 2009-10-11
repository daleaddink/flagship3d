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
		// ��ȡ��Դ
		virtual bool          Create();

		// �ͷ���Դ
		virtual void          Destory();

		// ��ȡ��������
		virtual Vector3f *    GetVertexData() { return m_kPMFMeshData.pPositions; }

		// ��ȡ��������
		virtual WORD *        GetIndexData() { return m_kPMFMeshData.pFaceIndices; }

		// ��ȡ��������
		virtual Vector3f *    GetNormalData() { return m_kPMFMeshData.pNormals; }
		
	private:
		
	};
}

#endif