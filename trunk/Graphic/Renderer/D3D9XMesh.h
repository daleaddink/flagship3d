#ifndef _D3D9XMESH_H_
#define _D3D9XMESH_H_

#include "D3D9Prerequisites.h"
#include "D3D9Mesh.h"
#include "../Main/Mesh.h"

namespace Flagship
{
	class _DLL_Export D3D9XMesh : public Mesh, public D3D9Mesh
	{
	public:
		D3D9XMesh();
		virtual ~D3D9XMesh();

		// ���㷨�߼�����
		bool                            ComputeMeshInfo();

	protected:
		// ��ȡ��Դ
		virtual bool                    Create();

		// �ͷ���Դ
		virtual void                    Destory();

		// ������Դ
		virtual bool                    Cache();

		// �ͷŻ���
		virtual void                    UnCache();

	protected:
		// D3D9ģ�Ͷ���
		LPD3DXMESH                      m_pD3D9Mesh;

	private:
	};

	typedef ResHandle<D3D9XMesh> D3D9XMeshHandle;
}

#endif