#ifndef _THREAD_H_
#define _THREAD_H_

#include "Prerequisites.h"

namespace Flagship
{
	class _DLL_Export Thread
	{
	public:
		Thread();
		virtual ~Thread();

		// 获取线程句柄
		HANDLE   GetThreadHandle();

		// 获取线程ID
		DWORD    GetThreadID();
		
		// 获取线程优先级
		int      GetThreadPriority();

		// 设置线程优先级
		bool     SetThreadPriority( int iPriority );

		// 挂起线程
		DWORD    SuspendThread();

		// 恢复线程
		DWORD    ResumeThread();

	public:
		// 执行线程
		virtual bool    StartThread();

		// 停止循环线程
		virtual void    StopThread();

		// 线程主函数
		virtual int     Run() = 0;

	protected:
		// 线程函数
		static UINT WINAPI      _ThreadProc( LPVOID lpParameter );

	protected:
		// 线程运行状态
		bool     m_bRunning;

		// 线程句柄
		HANDLE   m_hThread;

		// 线程ID
		DWORD    m_dwThreadID;
	};
}

#endif