#ifndef _D3D9OGREMESH_H_
#define _D3D9OGREMESH_H_

#include "D3D9Prerequisites.h"
#include "D3D9Mesh.h"
#include "../Main/OgreMesh.h"

namespace Flagship
{
	class _DLL_Export D3D9OgreMesh : public OgreMesh, public D3D9Mesh
	{
	public:
		D3D9OgreMesh();
		virtual ~D3D9OgreMesh();

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

	typedef ResHandle<D3D9OgreMesh> D3D9OgreMeshHandle;
}

#endif