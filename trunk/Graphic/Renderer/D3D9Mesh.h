#ifndef _D3D9MESH_H_
#define _D3D9MESH_H_

#include "D3D9Prerequisites.h"

namespace Flagship
{
	class _DLL_Export D3D9Mesh
	{
	public:
		D3D9Mesh();
		virtual ~D3D9Mesh();

		// 获取顶点格式描述
		LPDIRECT3DVERTEXDECLARATION9    GetVertexDeclaration() { return m_pD3D9VertexDecl; }		

		// 获取顶点缓存实例指针
		LPDIRECT3DVERTEXBUFFER9         GetVertexBuffer() { return m_pD3D9VertexBuffer; }

		// 获取索引缓存实例指针
		LPDIRECT3DINDEXBUFFER9          GetIndexBuffer() { return m_pD3D9IndexBuffer; }
		
	protected:

		// D3D9顶点格式描述
		LPDIRECT3DVERTEXDECLARATION9    m_pD3D9VertexDecl;

		// D3D9顶点缓存
		LPDIRECT3DVERTEXBUFFER9         m_pD3D9VertexBuffer;

		// D3D9索引缓存
		LPDIRECT3DINDEXBUFFER9          m_pD3D9IndexBuffer;

	private:
	};
}

#endif