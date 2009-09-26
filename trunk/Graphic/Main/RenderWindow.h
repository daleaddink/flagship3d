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

		// ���ô��ھ��
		void                     SetWindowHandle( HWND hHandle );

		// ��ȡ���ھ��
		HWND                     GetWindowHandle() { return m_hWindowHandle; }

		// ��ȡ����ʱ��
		float                    GetElapsedTime();

		// ��ȡ��ǰ֡
		DWORD                    GetCurrentFrame() { return m_dwCurFrame; }

		// ��ȡ��ǰ��Ⱦ����
		static void              SetActiveRenderWindow( RenderWindow * pRenderWindow );

		// ��ȡ��ǰ��Ⱦ����
		static RenderWindow *    GetActiveRenderWindow();

	public:
		// ��ʼ����Ⱦ����
		virtual bool             Initialize( bool bFullScreen = false, bool bHDR = false );

		// ������Ⱦ����
		virtual void             Update();

	protected:
		// ��ǰʱ��
		DWORD                    m_dwLastTime;

		// ����
		int                      m_iFrameCount;

		// ��ǰ֡
		DWORD                    m_dwCurFrame;

		// ��ǰFPS
		int                      m_iFPS;

		// ����ʱ��
		float                    m_fElapsedTime;

		// �ۼ�ʱ��
		float                    m_fSumTime;

		// ���ھ��
		HWND                     m_hWindowHandle;

		// ��̬��Ⱦ����
		static RenderWindow *    m_pActiveRenderWindow;		

	private:
	};
}

#endif