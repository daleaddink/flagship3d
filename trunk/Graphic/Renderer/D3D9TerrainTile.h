#ifndef _D3D9TERRAINTILE_H_
#define _D3D9TERRAINTILE_H_

#include "D3D9Prerequisites.h"
#include "D3D9Mesh.h"
#include "../Main/TerrainTile.h"

namespace Flagship
{
	class _DLL_Export D3D9TerrainTile : public TerrainTile, public D3D9Mesh
	{
	public:
		D3D9TerrainTile();
		virtual ~D3D9TerrainTile();

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
	};

	typedef ResHandle<D3D9TerrainTile> D3D9TerrainTileHandle;
}

#endif