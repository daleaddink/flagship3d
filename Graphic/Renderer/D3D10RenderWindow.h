#ifndef _D3D10RENDERWINDOW_H_
#define _D3D10RENDERWINDOW_H_

#include "D3D10Prerequisites.h"
#include "../Main/RenderWindow.h"

namespace Flagship
{
	struct ScreenVertex
	{
		D3DXVECTOR4                vPosition;
		D3DXVECTOR2                vTextureCoord;
		static const DWORD         FVF;
	};

	class _DLL_Export D3D10RenderWindow : public RenderWindow
	{
	public:
		D3D10RenderWindow();
		virtual ~D3D10RenderWindow();

		// �ָ�Դ��Ⱦ����
		void                        RestoreRenderTarget();

		// ��ȡ��Ⱦ����
		ID3D10Texture2D *           GetRenderTarget();

		// ��ȡ���ģ�����
		ID3D10Texture2D *           GetDepthStencil();

		// ��ȡ��Ⱦ����
		ID3D10RenderTargetView *    GetRenderTargetView();

		// ��ȡ���ģ�����
		ID3D10DepthStencilView *    GetDepthStencilView();

		// ��ȡ��Ļ���㻺��
		ID3D10Buffer *              GetScreenVertexBuffer();

		// ��ȡ��Ļ��������
		ID3D10InputLayout *         GetScreenVertexDeclaration( ID3D10EffectTechnique * pTech );

		// ��ȡD3D10�豸ָ��
		ID3D10Device *              GetDevice();

		// ��ȡD3D10������
		IDXGISwapChain *            GetSwapChain();
		
		// ��ȡ����
		ID3DX10Font *               GetFont();

		// ��ȡ���
		UINT                        GetWidth() { return m_uiWidth; }

		// ��ȡ�߶�
		UINT                        GetHeight() { return m_uiHeight; }

	public:
		// ��ʼ����Ⱦ����
		virtual bool                Initialize( bool bFullScreen = false, bool bHDR = false );

		// ������Ⱦ����
		virtual void                Update();

		// �洢��Ⱦ��ͼ
		virtual void                SaveRenderTexture( wstring szPath );

	protected:
		// D3D10������
		IDXGISwapChain *            m_pD3D10SwapChain;

		// D3D10�豸ָ��
		ID3D10Device *              m_pD3D10Device;

		// D3D10����
		ID3DX10Font *               m_pFont;

		// HDR״̬
		bool                        m_bHDR;

		// ���ڳ���
		UINT                        m_uiWidth;
		UINT                        m_uiHeight;

		// LDR��Ⱦ��
		ID3D10Texture2D *           m_pLDRTextureRT;
		ID3D10RenderTargetView *    m_pLDRSurfaceRT;
		ID3D10Texture2D *           m_pLDRTextureDS;
		ID3D10DepthStencilView *    m_pLDRSurfaceDS;

		// HDR��Ⱦ��
		ID3D10Texture2D *           m_pHDRTextureRT;
		ID3D10RenderTargetView *    m_pHDRSurfaceRT;
		ID3D10Texture2D *           m_pHDRTextureDS;
		ID3D10DepthStencilView *    m_pHDRSurfaceDS;

		// ��Ļ���㻺��
		ID3D10Buffer *              m_pScreenVertexBuffer;

		// ��Ļ�����ʽ����
		D3D10_INPUT_ELEMENT_DESC    m_pScreenInputElement[3];

		// ��Ļ��������
		ID3D10InputLayout *         m_pScreenInputLayout;

	private:

	};
}

#endif