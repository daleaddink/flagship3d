#ifndef _TERRAINTILE_H_
#define _TERRAINTILE_H_

#include <Common/Prerequisites.h>
#include "Mesh.h"

namespace Flagship
{
	class Texture;

	class _DLL_Export TerrainTile : public Mesh
	{
	public:
		TerrainTile();
		virtual ~TerrainTile();

		// 设置地形参数
		void                SetConfig( int iSizeX, int iSizeZ, int iLod );
		
	protected:
		// 读取资源
		virtual bool        Create();

		// 释放资源
		virtual void        Destory();
		
	protected:
		// 地形参数
		int                 m_iSizeX;
		int                 m_iSizeZ;
		int                 m_iLod;

		// 顶点列表
		Vector3f *          m_pVertexList;

		// 索引列表
		DWORD *             m_pIndexList;
				
	private:

	};
}

#endif