#ifndef _LOGIC_H_
#define _LOGIC_H_

#include <Common/Prerequisites.h>
#include <Common/Thread.h>

namespace Flagship
{
	class Camera;

	class _DLL_Export Logic : public Thread
	{
	public:
		Logic();
		virtual ~Logic();

		// 获取当前FPS
		int               GetFPS() { return m_iFPS; }

		// 获取经过时间
		float             GetElapsedTime() { return m_fElapsedTime; }

		// 获取摄像机
		Camera *          GetCamera() { return m_pCamera; }

	public:
		// 开始运行
		virtual int       Run();

		// 初始化
		virtual void      Initialize();

		// 逻辑函数
		virtual bool      LogicTick();

		// 消息处理
		virtual bool      MessageProc( UINT uMsg, WPARAM wParam, LPARAM lParam ) { return false; }

	protected:
		// 摄像机
		Camera *          m_pCamera;

		// 当前时间
		DWORD             m_dwLastTime;

		// 计数
		int               m_iFrameCount;
		
		// 当前FPS
		int               m_iFPS;

		// 经过时间
		float             m_fElapsedTime;

		// 累计时间
		float             m_fSumTime;

	private:

	};
}

#endif