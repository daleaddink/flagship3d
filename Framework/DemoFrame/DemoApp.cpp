#include "DemoApp.h"
#include "Graphic.h"
#include "../../Graphic/Main/Logic.h"
#include "../../Graphic/Main/RenderTargetQueue.h"
#include "../../Graphic/Main/ResourceLoader.h"

namespace Flagship
{
	Logic *    DemoApp::m_pLogic = NULL;

	DemoApp::DemoApp()
	{
#ifdef FLAGSHIP_DX10
		m_pRenderWindow = new D3D10RenderWindow;
#else
		m_pRenderWindow = new D3D9RenderWindow;
#endif
	}

	DemoApp::~DemoApp()
	{
		SAFE_DELETE( m_pRenderWindow );
	}

	int    DemoApp::Run( HINSTANCE hInstance, int nCmdShow )
	{
		WNDCLASS WndClass;
		WndClass.style			= CS_HREDRAW | CS_VREDRAW;
		WndClass.lpfnWndProc	= _WndProc;
		WndClass.cbClsExtra		= 0;
		WndClass.cbWndExtra		= 0;
		WndClass.hInstance		= hInstance;
		WndClass.hIcon			= LoadIcon( hInstance, IDI_APPLICATION );
		WndClass.hCursor		= LoadCursor( NULL, IDC_ARROW );
		WndClass.hbrBackground	= (HBRUSH)( COLOR_WINDOWFRAME );
		WndClass.lpszMenuName	= NULL;
		WndClass.lpszClassName	= L"Flagship";
		RegisterClass( &WndClass );

		m_hWnd = CreateWindow( L"Flagship", L"Flagship Demo", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

		if ( ! m_hWnd )
		{
			return 0;
		}

		ShowWindow( m_hWnd, nCmdShow );
		UpdateWindow( m_hWnd );

		// 加载渲染器
#ifdef FLAGSHIP_DX10
		LoadLibrary( L"D3D10Renderer.dll" );
#else
		LoadLibrary( L"D3D9Renderer.dll" );
#endif

		// 渲染窗口初始化
		m_pRenderWindow->SetWindowHandle( m_hWnd );
		m_pRenderWindow->SetCamera( m_pLogic->GetCamera() );
		m_pRenderWindow->Initialize( false, true );
		SceneManager::GetSingleton()->SetThreadInfo( GetCurrentThread(), GetCurrentThreadId() );
		RenderTargetQueue::GetSingleton()->AddRenderTarget( m_pRenderWindow );

		ResourceLoader::GetSingleton()->StartThread();

		// 逻辑初始化
		m_pLogic->Initialize();

#ifndef FLAGSHIP_ST
		m_pLogic->StartThread();
		
		// 设置运行CPU
		SetThreadAffinityMask( GetCurrentThread(), 1L );
		SetThreadAffinityMask( m_pLogic->GetThreadHandle(), 2L );
#endif

		MSG msg;
		vector<MSG> pFailList;

		while ( 1 )
		{
			if ( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
			{
				if ( ! GetMessage( &msg, NULL, 0, 0 ) )
				{
					return (int) msg.wParam;
				}

				if ( IsRenderMsg( msg.message ) )
				{
					if ( ! SceneManager::GetSingleton()->MessageProc( msg.message, msg.wParam, msg.lParam ) )
					{
						pFailList.push_back( msg );
					}
				}
				else
				{
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
			}

			RenderTargetQueue::GetSingleton()->UpdateAll();

			if ( pFailList.size() != 0 )
			{
				for ( int i = 0; i < (int) pFailList.size(); i++ )
				{
					PostMessage( m_hWnd, pFailList[i].message, pFailList[i].wParam, pFailList[i].lParam );
				}

				pFailList.clear();
			}

#ifdef FLAGSHIP_ST
			m_pLogic->LogicTick();
#endif
		}

		return (int) msg.wParam;
	}

	bool    DemoApp::IsLogicMsg( UINT uMsg )
	{
		if ( uMsg == WM_KEYDOWN )
		{
			return true;
		}

		if ( uMsg == WM_KEYUP )
		{
			return true;
		}

		if ( uMsg == WM_RBUTTONDOWN )
		{
			return true;
		}

		if ( uMsg == WM_RBUTTONUP )
		{
			return true;
		}

		if ( uMsg == WM_MOUSEMOVE )
		{
			return true;
		}

		return false;
	}

	bool    DemoApp::IsRenderMsg( UINT uMsg )
	{
		switch ( uMsg )
		{
		case WM_ADDENTITY:
		case WM_UPDATEENTITY:
		case WM_DELENTITY:
			{
				return true;
				break;
			}
		default:
			{
				return false;
				break;
			}
		}

		return false;
	}

	LRESULT     DemoApp::_WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		PAINTSTRUCT ps;
		HDC hdc;

		switch ( uMsg )
		{
		case WM_PAINT:
			{
				hdc = BeginPaint(hWnd, &ps);
				EndPaint(hWnd, &ps);
			}
			break;
		case WM_DESTROY:
			{
				m_pLogic->StopThread();
				WaitForSingleObject( m_pLogic->GetThreadHandle(), INFINITE );
				PostQuitMessage(0);
			}
			break;
		default:
			{
				if ( IsLogicMsg( uMsg ) )
				{
#ifdef FLAGSHIP_ST
					m_pLogic->MessageProc( uMsg, wParam, lParam );
#else
					PostThreadMessage( m_pLogic->GetThreadID(), uMsg, wParam, lParam );
#endif
				}
				else
				{
    				return DefWindowProc( hWnd, uMsg, wParam, lParam );
				}
			}
			break;
		}

		return 0;
	}
}