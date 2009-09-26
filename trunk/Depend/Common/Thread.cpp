#include "Thread.h"

namespace Flagship
{
	Thread::Thread()
	{
		m_hThread = NULL;
		m_dwThreadID = 0;
		m_bRunning = true;
	}

	Thread::~Thread()
	{
		CloseHandle( m_hThread );
	}

	HANDLE    Thread::GetThreadHandle()
	{
		return m_hThread;
	}

	DWORD    Thread::GetThreadID()
	{
		return m_dwThreadID;
	}

	bool   Thread::StartThread()
	{
		m_hThread = ::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE) _ThreadProc, (LPVOID) this, 0, &m_dwThreadID );
		
		if ( m_hThread == INVALID_HANDLE_VALUE )
		{
			return false;
		}

		return true;
	}

	void   Thread::StopThread()
	{
		m_bRunning = false;
	}

	int    Thread::GetThreadPriority()
	{
		return ::GetThreadPriority(m_hThread);
	}

	bool   Thread::SetThreadPriority( int iPriority )
	{
		return ::SetThreadPriority( m_hThread, iPriority );
	}

	DWORD  Thread::SuspendThread()
	{
		return ::SuspendThread( m_hThread );
	}

	DWORD  Thread::ResumeThread()
	{
		return ::ResumeThread( m_hThread );
	}

	UINT   Thread::_ThreadProc( LPVOID pParam )
	{
		Thread * pThread = (Thread *) pParam;
		pThread->Run();

		return 0;
	}
}