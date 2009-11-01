#ifndef _D3D10SHADOWMAP_H_
#define _D3D10SHADOWMAP_H_

#include "D3D10Prerequisites.h"
#include "../Main/ShadowMap.h"

namespace Flagship
{
	class _DLL_Export D3D10ShadowMap : public ShadowMap
	{
	public:
		D3D10ShadowMap();
		virtual ~D3D10ShadowMap();

	public:
		// ��ʼ����Ⱦ��ͼ
		virtual bool                Initialize( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// ������Ӱͼ
		virtual void                UpdateShadow( Light * pLight, BYTE byShadowFace = Shadow_Max );

		// �洢��Ⱦ��ͼ
		virtual void                SaveRenderTexture( wstring szPath );

	protected:
		// ��Ⱦ����
		ID3D10RenderTargetView *    m_pRenderSurface;

		// ���ģ�����
		ID3D10DepthStencilView *    m_pDepthSurface;

	private:
	};
}

#endif