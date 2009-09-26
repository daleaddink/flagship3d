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

		// ����
		int                     Run( HINSTANCE hInstance, int nCmdShow );

		// ��ȡ���ھ��
		HWND                    GetWindowHandle() { return m_hWnd; }

		// ��ȡ�߼�
		Logic *                 GetLogic() { return m_pLogic; }

	protected:
		// ��Ⱦ����
		RenderWindow *          m_pRenderWindow;

		// ���ھ��
		HWND                    m_hWnd;

		// �߼�
		static Logic *          m_pLogic;
				
	private:
		// �Ƿ�Ϊ�߼�������Ϣ
		static bool             IsLogicMsg( UINT uMsg );

		// �Ƿ�Ϊ��Ⱦ������Ϣ
		static bool             IsRenderMsg( UINT uMsg );

		// ��Ϣ����
		static LRESULT CALLBACK _WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	};
}

#endif