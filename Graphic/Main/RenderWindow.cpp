#include "RenderWindow.h"
#include "Renderer.h"
#include "PostProcessQueue.h"
#include "ResourceManager.h"

namespace Flagship
{
	RenderWindow *    RenderWindow::m_pActiveRenderWindow = NULL;

	RenderWindow::RenderWindow()
	{
		m_dwLastTime = GetTickCount();
		m_fElapsedTime = 0.0f;
		m_fSumTime = 0.0f;
		m_iFrameCount = 0;
		m_dwCurFrame = 0;
		m_iFPS = 0;

		m_iClassType = Base::RenderTarget_Window;
	}

	RenderWindow::~RenderWindow()
	{

	}

	void    RenderWindow::SetWindowHandle( HWND hHandle )
	{
		m_hWindowHandle = hHandle;
	}

	float    RenderWindow::GetElapsedTime()
	{
		return m_fElapsedTime;
	}

	void    RenderWindow::SetActiveRenderWindow( RenderWindow * pRenderWindow )
	{
		m_pActiveRenderWindow = pRenderWindow;
	}

	RenderWindow *    RenderWindow::GetActiveRenderWindow()
	{
		return m_pActiveRenderWindow;
	}

	bool    RenderWindow::Initialize( bool bFullScreen, bool bHDR )
	{
		return true;
	}

	void    RenderWindow::Update()
	{
		// ���õ�ǰ��Ⱦ����
		m_pActiveRenderWindow = this;

		// ��ʼ����
		if ( m_pRenderer->BeginScene() )
		{
			// ���µ�ǰ֡
			m_dwCurFrame ++;

			// ������Ⱦ����
			RenderTarget::Update();

			// ����
			PostProcessQueue::GetSingleton()->ProcessAll();

			// ���»��������
			ResourceManager::GetSingleton()->Update();

			// ����ʱ��
			m_fElapsedTime = (float) ( GetTickCount() - m_dwLastTime ) / 1000.0f;
			m_dwLastTime = GetTickCount();
			m_fSumTime += m_fElapsedTime;
			m_iFrameCount ++;
			
			// ��ȾFPS
			if ( m_fSumTime > 1.0f )
			{
				m_iFPS = (int) ( m_iFrameCount / m_fSumTime );
				m_iFrameCount = 0;
				m_fSumTime = 0.0f;
			}

			wchar_t szText[32];
			_itow_s( m_iFPS, szText, 32, 10 );
			wstring szFPS = szText;
			m_pRenderer->RenderText( 5, 5, Vector4f( 1.0f, 1.0f, 0.0f, 1.0f ), szFPS );

			int iY = 25;
			RenderQueue * pQueue = m_pRenderer->GetRenderQueue( Renderer::RenderQueue_Visible );
			multimap< float, Entity * >::iterator VisibleIt = pQueue->GetEntityList()->begin();
			while ( VisibleIt != pQueue->GetEntityList()->end() )
			{
				Key szKey = (*VisibleIt).second->GetKey();
				m_pRenderer->RenderText( 5, iY, Vector4f( 1.0f, 1.0f, 0.0f, 1.0f ), szKey.GetName() );

				iY += 20;
				VisibleIt++;
			}

			// ��������
			m_pRenderer->EndScene();
		}

		// ���彻��
		m_pRenderer->Present();
	}
}