#ifndef _D3D9RENDERWINDOW_H_
#define _D3D9RENDERWINDOW_H_

#include "D3D9Prerequisites.h"

#include "../Main/RenderWindow.h"

namespace Flagship
{
	class _DLL_Export D3D9RenderWindow : public RenderWindow
	{
	public:
		D3D9RenderWindow();
		virtual ~D3D9RenderWindow();

		// �ָ�Դ��Ⱦ����
		void                  RestoreRenderTarget();

		// ��ȡ��Ⱦ����
		LPDIRECT3DSURFACE9    GetRenderTarget();

		// ��ȡ���ģ�����
		LPDIRECT3DSURFACE9    GetDepthStencil();

		// ��ȡD3D9�豸ָ��
		LPDIRECT3DDEVICE9     GetDevice();

		// ��ȡ����
		LPD3DXFONT            GetFont();

	public:
		// ��ʼ����Ⱦ����
		virtual bool          Initialize( bool bFullScreen = false, bool bHDR = false );

		// ������Ⱦ����
		virtual void          Update();

		// �洢��Ⱦ��ͼ
		virtual void          SaveRenderTexture( wstring szPath );

	protected:
		// D3D9ָ��
		LPDIRECT3D9           m_pD3D;

		// D3D9�豸ָ��
		LPDIRECT3DDEVICE9     m_pD3D9Device;

		// D3D9����
		LPD3DXFONT            m_pFont;

		// HDR״̬
		bool                  m_bHDR;

		// LDR��Ⱦ��
		LPDIRECT3DSURFACE9    m_pLDRSurfaceRT;
		LPDIRECT3DSURFACE9    m_pLDRSurfaceDS;

		// HDR��Ⱦ��
		LPDIRECT3DSURFACE9    m_pHDRSurfaceRT;
		LPDIRECT3DSURFACE9    m_pHDRSurfaceDS;

	private:
	};
}

#endif