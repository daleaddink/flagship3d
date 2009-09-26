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

		// 恢复源渲染对象
		void                        RestoreRenderTarget();

		// 获取渲染对象
		ID3D10Texture2D *           GetRenderTarget();

		// 获取深度模版对象
		ID3D10Texture2D *           GetDepthStencil();

		// 获取渲染表面
		ID3D10RenderTargetView *    GetRenderTargetView();

		// 获取深度模版表面
		ID3D10DepthStencilView *    GetDepthStencilView();

		// 获取屏幕顶点缓冲
		ID3D10Buffer *              GetScreenVertexBuffer();

		// 获取屏幕顶点描述
		ID3D10InputLayout *         GetScreenVertexDeclaration( ID3D10EffectTechnique * pTech );

		// 获取D3D10设备指针
		ID3D10Device *              GetDevice();

		// 获取D3D10缓冲链
		IDXGISwapChain *            GetSwapChain();
		
		// 获取字体
		ID3DX10Font *               GetFont();

		// 获取宽度
		UINT                        GetWidth() { return m_uiWidth; }

		// 获取高度
		UINT                        GetHeight() { return m_uiHeight; }

	public:
		// 初始化渲染窗口
		virtual bool                Initialize( bool bFullScreen = false, bool bHDR = false );

		// 更新渲染对象
		virtual void                Update();

		// 存储渲染贴图
		virtual void                SaveRenderTexture( wstring szPath );

	protected:
		// D3D10缓冲链
		IDXGISwapChain *            m_pD3D10SwapChain;

		// D3D10设备指针
		ID3D10Device *              m_pD3D10Device;

		// D3D10字体
		ID3DX10Font *               m_pFont;

		// HDR状态
		bool                        m_bHDR;

		// 窗口长宽
		UINT                        m_uiWidth;
		UINT                        m_uiHeight;

		// LDR渲染面
		ID3D10Texture2D *           m_pLDRTextureRT;
		ID3D10RenderTargetView *    m_pLDRSurfaceRT;
		ID3D10Texture2D *           m_pLDRTextureDS;
		ID3D10DepthStencilView *    m_pLDRSurfaceDS;

		// HDR渲染面
		ID3D10Texture2D *           m_pHDRTextureRT;
		ID3D10RenderTargetView *    m_pHDRSurfaceRT;
		ID3D10Texture2D *           m_pHDRTextureDS;
		ID3D10DepthStencilView *    m_pHDRSurfaceDS;

		// 屏幕顶点缓冲
		ID3D10Buffer *              m_pScreenVertexBuffer;

		// 屏幕顶点格式描述
		D3D10_INPUT_ELEMENT_DESC    m_pScreenInputElement[3];

		// 屏幕顶点描述
		ID3D10InputLayout *         m_pScreenInputLayout;

	private:

	};
}

#endif