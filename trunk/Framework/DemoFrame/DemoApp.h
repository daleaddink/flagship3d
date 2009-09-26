#ifndef _DEMOAPP_H_
#define _DEMOAPP_H_

#include <Common/Prerequisites.h>

namespace Flagship
{
	class RenderWindow;
	class Logic;

	class DemoApp
	{
	public:
		DemoApp();
		virtual ~DemoApp();

		// 运行
		int                     Run( HINSTANCE hInstance, int nCmdShow );

		// 获取窗口句柄
		HWND                    GetWindowHandle() { return m_hWnd; }

		// 获取逻辑
		Logic *                 GetLogic() { return m_pLogic; }

	protected:
		// 渲染窗口
		RenderWindow *          m_pRenderWindow;

		// 窗口句柄
		HWND                    m_hWnd;

		// 逻辑
		static Logic *          m_pLogic;
				
	private:
		// 是否为逻辑处理消息
		static bool             IsLogicMsg( UINT uMsg );

		// 是否为渲染处理消息
		static bool             IsRenderMsg( UINT uMsg );

		// 消息处理
		static LRESULT CALLBACK _WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	};
}

#endif