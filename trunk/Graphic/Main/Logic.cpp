#include "Logic.h"
#include "Camera.h"

namespace Flagship
{
	Logic::Logic()
	{
		m_dwLastTime = GetTickCount();
		m_fElapsedTime = 0.0f;
		m_fSumTime = 0.0f;
		m_iFrameCount = 0;
		m_iFPS = 0;
	}

	Logic::~Logic()
	{

	}

	int    Logic::Run()
	{
		MSG msg;
		while ( m_bRunning )
		{
			if ( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
			{
				if ( ! GetMessage( &msg, NULL, 0, 0 ) )
				{
					return (int) msg.wParam;
				}

				MessageProc( msg.message, msg.wParam, msg.lParam );
			}

			LogicTick();
		}

		return 0;
	}

	void    Logic::Initialize()
	{
		MSG msg;
		PeekMessage( &msg, NULL, WM_USER, WM_USER, PM_NOREMOVE );
	}

	bool    Logic::LogicTick()
	{
		// 计算时间
		m_fElapsedTime = (float) ( GetTickCount() - m_dwLastTime ) / 1000.0f;
		m_dwLastTime = GetTickCount();
		m_fSumTime += m_fElapsedTime;
		m_iFrameCount ++;

		if ( m_fSumTime < 1.0f / LOGIC_FPS )
		{
			return false;
		}

		// 计算FPS
		if ( m_fSumTime > 1.0f )
		{
			m_iFPS = LOGIC_FPS;
			m_iFrameCount = 0;
			m_fSumTime = 0.0f;
		}

		return true;
	}
}