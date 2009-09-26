#ifndef _D3D9RENDERTEXTURE_H_
#define _D3D9RENDERTEXTURE_H_

#include "D3D9Prerequisites.h"
#include "../Main/RenderTexture.h"

namespace Flagship
{
	class _DLL_Export D3D9RenderTexture : public RenderTexture
	{
	public:
		D3D9RenderTexture();
		virtual ~D3D9RenderTexture();

		// ��ʼ����Ⱦ��ͼ
		bool                  Initialize( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

	public:
		// ������Ⱦ����
		virtual void          Update();

		// �洢��Ⱦ��ͼ
		virtual void          SaveRenderTexture( wstring szPath );

	protected:
		// ��Ⱦ����
		LPDIRECT3DSURFACE9    m_pRenderSurface;

		// ���ģ�����
		LPDIRECT3DSURFACE9    m_pDepthSurface;

	private:
	};
}

#endif