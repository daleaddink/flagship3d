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

		// 计算法线及切线
		bool                            ComputeMeshInfo();

	protected:
		// 读取资源
		virtual bool                    Create();

		// 释放资源
		virtual void                    Destory();

		// 缓存资源
		virtual bool                    Cache();

		// 释放缓存
		virtual void                    UnCache();

	protected:
		// D3D9模型对象
		LPD3DXMESH                      m_pD3D9Mesh;

	private:
	};

	typedef ResHandle<D3D9XMesh> D3D9XMeshHandle;
}

#endif