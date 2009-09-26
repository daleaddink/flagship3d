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

		// ��ȡ��������
		DWORD                 GetNumTriangle() { return m_dwNumTrangle; }

		// ��ȡ������
		DWORD                 GetNumVertex() { return m_dwNumVertex; }

		// ��ȡ�����С
		DWORD                 GetVertexSize() { return m_dwVertexSize; }

	public:
	    // ��ȡ��������
		virtual Vector3f *    GetVertexData() { return NULL; }

		// ��ȡ��������
		virtual WORD *        GetIndexData() { return NULL; }

		// ��ȡ��������
		virtual Vector3f *    GetNormalData() { return NULL; }

	protected:
		// �����ʽ
		DWORD                 m_dwFVF;

		// ��������
		DWORD                 m_dwNumTrangle;

		// ������
		DWORD                 m_dwNumVertex;

		// �����С
		DWORD                 m_dwVertexSize;

		// ��Χ��
		BoundingVolume *      m_pBound;

	private:
	};
}

#endif