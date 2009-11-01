#ifndef _D3D10PMFMESH_H_
#define _D3D10PMFMESH_H_

#include "D3D10Prerequisites.h"
#include "D3D10Mesh.h"
#include "../Main/PMFMesh.h"

namespace Flagship
{
	class _DLL_Export D3D10PMFMesh : public PMFMesh, public D3D10Mesh
	{
	public:
		D3D10PMFMesh();
		virtual ~D3D10PMFMesh();

	protected:
		// ��ȡ��Դ
		virtual bool                    Create();

		// ������Դ
		virtual bool                    Cache();

		// �ͷŻ���
		virtual void                    UnCache();

	protected:

	private:
	};

	typedef ResHandle<D3D10PMFMesh> D3D10PMFMeshHandle;
}

#endif