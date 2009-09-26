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

		// ���õ��β���
		void                SetConfig( int iSizeX, int iSizeZ, int iLod );
		
	protected:
		// ��ȡ��Դ
		virtual bool        Create();

		// �ͷ���Դ
		virtual void        Destory();
		
	protected:
		// ���β���
		int                 m_iSizeX;
		int                 m_iSizeZ;
		int                 m_iLod;

		// �����б�
		Vector3f *          m_pVertexList;

		// �����б�
		DWORD *             m_pIndexList;
				
	private:

	};
}

#endif