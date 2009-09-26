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

		// ��ȡ�߳̾��
		HANDLE   GetThreadHandle();

		// ��ȡ�߳�ID
		DWORD    GetThreadID();
		
		// ��ȡ�߳����ȼ�
		int      GetThreadPriority();

		// �����߳����ȼ�
		bool     SetThreadPriority( int iPriority );

		// �����߳�
		DWORD    SuspendThread();

		// �ָ��߳�
		DWORD    ResumeThread();

	public:
		// ִ���߳�
		virtual bool    StartThread();

		// ֹͣѭ���߳�
		virtual void    StopThread();

		// �߳�������
		virtual int     Run() = 0;

	protected:
		// �̺߳���
		static UINT WINAPI      _ThreadProc( LPVOID lpParameter );

	protected:
		// �߳�����״̬
		bool     m_bRunning;

		// �߳̾��
		HANDLE   m_hThread;

		// �߳�ID
		DWORD    m_dwThreadID;
	};
}

#endif