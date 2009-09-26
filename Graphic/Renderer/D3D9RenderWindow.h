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

		// 恢复源渲染对象
		void                  RestoreRenderTarget();

		// 获取渲染表面
		LPDIRECT3DSURFACE9    GetRenderTarget();

		// 获取深度模版表面
		LPDIRECT3DSURFACE9    GetDepthStencil();

		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9     GetDevice();

		// 获取字体
		LPD3DXFONT            GetFont();

	public:
		// 初始化渲染窗口
		virtual bool          Initialize( bool bFullScreen = false, bool bHDR = false );

		// 更新渲染对象
		virtual void          Update();

		// 存储渲染贴图
		virtual void          SaveRenderTexture( wstring szPath );

	protected:
		// D3D9指针
		LPDIRECT3D9           m_pD3D;

		// D3D9设备指针
		LPDIRECT3DDEVICE9     m_pD3D9Device;

		// D3D9字体
		LPD3DXFONT            m_pFont;

		// HDR状态
		bool                  m_bHDR;

		// LDR渲染面
		LPDIRECT3DSURFACE9    m_pLDRSurfaceRT;
		LPDIRECT3DSURFACE9    m_pLDRSurfaceDS;

		// HDR渲染面
		LPDIRECT3DSURFACE9    m_pHDRSurfaceRT;
		LPDIRECT3DSURFACE9    m_pHDRSurfaceDS;

	private:
	};
}

#endif