#ifndef _D3D9PMFMESH_H_
#define _D3D9PMFMESH_H_

#include "D3D9Prerequisites.h"
#include "D3D9Mesh.h"
#include "../Main/PMFMesh.h"

namespace Flagship
{
	class _DLL_Export D3D9PMFMesh : public PMFMesh, public D3D9Mesh
	{
	public:
		D3D9PMFMesh();
		virtual ~D3D9PMFMesh();

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

	private:
	};

	typedef ResHandle<D3D9PMFMesh> D3D9PMFMeshHandle;
}

#endif