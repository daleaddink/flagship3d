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

		// ��ȡ��ǰFPS
		int               GetFPS() { return m_iFPS; }

		// ��ȡ����ʱ��
		float             GetElapsedTime() { return m_fElapsedTime; }

		// ��ȡ�����
		Camera *          GetCamera() { return m_pCamera; }

	public:
		// ��ʼ����
		virtual int       Run();

		// ��ʼ��
		virtual void      Initialize();

		// �߼�����
		virtual bool      LogicTick();

		// ��Ϣ����
		virtual bool      MessageProc( UINT uMsg, WPARAM wParam, LPARAM lParam ) { return false; }

	protected:
		// �����
		Camera *          m_pCamera;

		// ��ǰʱ��
		DWORD             m_dwLastTime;

		// ����
		int               m_iFrameCount;
		
		// ��ǰFPS
		int               m_iFPS;

		// ����ʱ��
		float             m_fElapsedTime;

		// �ۼ�ʱ��
		float             m_fSumTime;

	private:

	};
}

#endif