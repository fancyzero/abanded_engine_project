#include "stdafx.h"
#include "resource.h"
#include "mod.h"
#include "mymod.h"

using namespace mania;
HINSTANCE	g_instance;
HWND		g_mainwnd;
mod*		g_mod;

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
ATOM MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WWW));
	wcex.hCursor		= LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= _T( "CMania" );
	wcex.hIconSm		= NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	return RegisterClassEx( &wcex );
}

BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	g_instance = hInstance; // 将实例句柄存储在全局变量中
	g_mainwnd = CreateWindow( _T( "CMania" ), _T( "Mania Test" ), WS_OVERLAPPEDWINDOW,
	                          CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, g_instance, NULL );
	if ( !g_mainwnd )
	{
		return FALSE;
	}
	ShowWindow( g_mainwnd, nCmdShow );
	UpdateWindow( g_mainwnd );
	return TRUE;
}

LRESULT on_resize( WPARAM wparam, LPARAM lparam )
{
	if ( get_core()->get_rendering_thread() )
	{
		mania::renderer_create_param p = get_core()->get_rendering_thread()->get_renderdevice()->get_create_param();
		p.w = LOWORD( lparam );
		p.h = HIWORD( lparam );
		if ( p.w == 0 || p.h == 0 )
			return 0;
		class resetdevice : public render_task
		{
			mania::renderer_create_param m_cp;
		public:
			resetdevice( rendering_thread* pRenderingThread, mania::renderer_create_param p1 )
					: render_task( pRenderingThread )
			{
				m_cp = p1;
			}
			DWORD execute()
			{
				if ( get_core()->get_rendering_thread() )
					if ( get_core()->get_rendering_thread()->get_renderdevice() )
					{
						return get_core()->get_rendering_thread()->get_renderdevice()->reset( m_cp );
					}
				return grc_failed;
			}
		};
		resetdevice* newtask = new resetdevice( mania::get_core()->get_rendering_thread(), p );
		mania::get_core()->get_rendering_thread()->AddRenderTask( newtask );
	}
	return 0;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message )
	{
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	case WM_SIZE:
		if ( wParam != SIZE_MINIMIZED )
			on_resize( wParam, lParam );
		break;
	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}

int WINAPI _tWinMain( HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow )
{
	MSG msg;
	MyRegisterClass( hInstance );
	if ( !InitInstance( hInstance, nCmdShow ) )
		return FALSE;
	mania::get_core()->parse_cmdline( lpstrCmdLine );
	g_mod = new mymod( g_instance, g_mainwnd );
	g_mod->init();
	int framecnt = 0;
	bool breakout = false;
	while ( !breakout )
	{
		get_core()->get_log()->write( 0, 0x00ff0000, _T( "[main thread]:   start frame: %d\n" ), framecnt );
		BOOL havemessage = PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE );
		if ( havemessage )
		{
			if ( !GetMessage( &msg, NULL, 0, 0 ) )
				breakout = true;
			else
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		if ( !havemessage || breakout )
		{
			if ( get_core()->main_thread_lock.wait( 0 ) == thread_wait_obj0 )
			{
				get_core()->main_thread_lock.reset();
				get_core()->render_thread_lock.set();
				if ( breakout )
				{
					get_core()->get_log()->write( 1, 0x00ff0000, _T( "Exit...\n" ) );
					g_mod->uninit();
					delete g_mod;
					mania::get_core()->uninit();
					if ( get_core()->main_thread_lock.wait( 0 ) == thread_wait_obj0 )
					{
						get_core()->main_thread_lock.reset();
						get_core()->render_thread_lock.set();
					}
					break;
				}
				framecnt++;
				g_mod->frame();
				get_core()->get_log()->write( 0, 0x00ff0000, _T( "[main thread]:   end frame: %d\n" ), framecnt );
			}
		}
	}
	return ( int ) msg.wParam;
}