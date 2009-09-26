#ifndef _D3D10MESH_H_
#define _D3D10MESH_H_

#include "D3D10Prerequisites.h"
#include "../Main/Base.h"

namespace Flagship
{
	class _DLL_Export D3D10Mesh
	{
	public:
		D3D10Mesh();
		virtual ~D3D10Mesh();

		// 获取顶点格式描述
		ID3D10InputLayout *                GetVertexDeclaration( Key& szTech, ID3D10EffectTechnique * pTech );

		// 获取顶点缓存实例指针
		ID3D10Buffer *                     GetVertexBuffer() { return m_pD3D10VertexBuffer; }

		// 获取索引缓存实例指针
		ID3D10Buffer *                     GetIndexBuffer() { return m_pD3D10IndexBuffer; }

		// 获取索引格式
		DXGI_FORMAT                        GetIndexFormat() { return m_dwIndexFormat; }

	protected:
		// 模型顶点格式描述
		D3D10_INPUT_ELEMENT_DESC           m_pInputElement[256];

		// 描述行数
		int                                m_iElementNum;

		// D3D10顶点格式描述
		map< Key, ID3D10InputLayout * >    m_pD3D10VertexDecl;

		// D3D10顶点缓存
		ID3D10Buffer *                     m_pD3D10VertexBuffer;

		// D3D10索引缓存
		ID3D10Buffer *                     m_pD3D10IndexBuffer;

		// 索引格式
		DXGI_FORMAT                        m_dwIndexFormat;

	private:
	};
}

#endif