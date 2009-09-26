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

		// ��ȡ�����ʽ����
		LPDIRECT3DVERTEXDECLARATION9    GetVertexDeclaration() { return m_pD3D9VertexDecl; }		

		// ��ȡ���㻺��ʵ��ָ��
		LPDIRECT3DVERTEXBUFFER9         GetVertexBuffer() { return m_pD3D9VertexBuffer; }

		// ��ȡ��������ʵ��ָ��
		LPDIRECT3DINDEXBUFFER9          GetIndexBuffer() { return m_pD3D9IndexBuffer; }
		
	protected:

		// D3D9�����ʽ����
		LPDIRECT3DVERTEXDECLARATION9    m_pD3D9VertexDecl;

		// D3D9���㻺��
		LPDIRECT3DVERTEXBUFFER9         m_pD3D9VertexBuffer;

		// D3D9��������
		LPDIRECT3DINDEXBUFFER9          m_pD3D9IndexBuffer;

	private:
	};
}

#endif