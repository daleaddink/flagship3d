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

		// ��ȡ�����ʽ����
		ID3D10InputLayout *                GetVertexDeclaration( Key& szTech, ID3D10EffectTechnique * pTech );

		// ��ȡ���㻺��ʵ��ָ��
		ID3D10Buffer *                     GetVertexBuffer() { return m_pD3D10VertexBuffer; }

		// ��ȡ��������ʵ��ָ��
		ID3D10Buffer *                     GetIndexBuffer() { return m_pD3D10IndexBuffer; }

		// ��ȡ������ʽ
		DXGI_FORMAT                        GetIndexFormat() { return m_dwIndexFormat; }

	protected:
		// ģ�Ͷ����ʽ����
		D3D10_INPUT_ELEMENT_DESC           m_pInputElement[256];

		// ��������
		int                                m_iElementNum;

		// D3D10�����ʽ����
		map< Key, ID3D10InputLayout * >    m_pD3D10VertexDecl;

		// D3D10���㻺��
		ID3D10Buffer *                     m_pD3D10VertexBuffer;

		// D3D10��������
		ID3D10Buffer *                     m_pD3D10IndexBuffer;

		// ������ʽ
		DXGI_FORMAT                        m_dwIndexFormat;

	private:
	};
}

#endif