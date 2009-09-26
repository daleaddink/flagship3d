#ifndef _D3D10SDKMESH_H_
#define _D3D10SDKMESH_H_

#include "D3D10Prerequisites.h"
#include "D3D10Mesh.h"
#include "DXUTSDKMesh.h"
#include "../Main/Mesh.h"

namespace Flagship
{
	class _DLL_Export D3D10SDKMesh : public Mesh, public D3D10Mesh
	{
	public:
		D3D10SDKMesh();
		virtual ~D3D10SDKMesh();

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

	private:
		// DXUT对象
		CDXUTSDKMesh *                  m_pSDKMesh;
	};

	typedef ResHandle<D3D10SDKMesh> D3D10SDKMeshHandle;
}

#endif