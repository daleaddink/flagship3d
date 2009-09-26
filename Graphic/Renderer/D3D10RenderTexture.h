#ifndef _D3D10RENDERTEXTURE_H_
#define _D3D10RENDERTEXTURE_H_

#include "D3D10Prerequisites.h"
#include "../Main/RenderTexture.h"

namespace Flagship
{
	class _DLL_Export D3D10RenderTexture : public RenderTexture
	{
	public:
		D3D10RenderTexture();
		virtual ~D3D10RenderTexture();

		// ��ʼ����Ⱦ��ͼ
		bool                        Initialize( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

	public:
		// �洢��Ⱦ��ͼ
		virtual void                SaveRenderTexture( wstring szPath );

		// ������Ⱦ����
		virtual void                Update();

	protected:
		// ��Ⱦ����
		ID3D10RenderTargetView *    m_pRenderSurface;

		// ���ģ�����
		ID3D10DepthStencilView *    m_pDepthSurface;

	private:
	};
}

#endif