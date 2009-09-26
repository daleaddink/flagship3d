#ifndef _D3D9SHADOWMAP_H_
#define _D3D9SHADOWMAP_H_

#include "D3D9Prerequisites.h"
#include "../Main/ShadowMap.h"

namespace Flagship
{
	class _DLL_Export D3D9ShadowMap : public ShadowMap
	{
	public:
		D3D9ShadowMap();
		virtual ~D3D9ShadowMap();
		
	public:
		// ��ʼ����Ⱦ��ͼ
		virtual bool          Initialize( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// ������Ӱͼ
		virtual void          UpdateShadow( Light * pLight, BYTE byShadowFace = Shadow_Max );

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