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
		// 读取资源
		virtual bool                    Create();

		// 缓存资源
		virtual bool                    Cache();

		// 释放缓存
		virtual void                    UnCache();

	protected:

	private:
	};

	typedef ResHandle<D3D10PMFMesh> D3D10PMFMeshHandle;
}

#endif