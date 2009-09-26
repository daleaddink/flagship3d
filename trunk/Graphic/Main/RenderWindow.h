#ifndef _RENDERWINDOW_H_
#define _RENDERWINDOW_H_

#include <Common/Prerequisites.h>
#include "RenderTarget.h"

namespace Flagship
{
	class _DLL_Export RenderWindow : public RenderTarget
	{
	public:
		RenderWindow();
		virtual ~RenderWindow();

		// 设置窗口句柄
		void                     SetWindowHandle( HWND hHandle );

		// 获取窗口句柄
		HWND                     GetWindowHandle() { return m_hWindowHandle; }

		// 获取经过时间
		float                    GetElapsedTime();

		// 获取当前帧
		DWORD                    GetCurrentFrame() { return m_dwCurFrame; }

		// 获取当前渲染窗口
		static void              SetActiveRenderWindow( RenderWindow * pRenderWindow );

		// 获取当前渲染窗口
		static RenderWindow *    GetActiveRenderWindow();

	public:
		// 初始化渲染窗口
		virtual bool             Initialize( bool bFullScreen = false, bool bHDR = false );

		// 更新渲染对象
		virtual void             Update();

	protected:
		// 当前时间
		DWORD                    m_dwLastTime;

		// 计数
		int                      m_iFrameCount;

		// 当前帧
		DWORD                    m_dwCurFrame;

		// 当前FPS
		int                      m_iFPS;

		// 经过时间
		float                    m_fElapsedTime;

		// 累计时间
		float                    m_fSumTime;

		// 窗口句柄
		HWND                     m_hWindowHandle;

		// 静态渲染窗口
		static RenderWindow *    m_pActiveRenderWindow;		

	private:
	};
}

#endif