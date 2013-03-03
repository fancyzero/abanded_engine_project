#include "stdafx.h"
#include "renderingthread.h"
#include "plugins\renderdevice\renderdevice.h"

namespace mania
{
DWORD WINAPI  RenderingThreadMain( void* pParam )
{
	DWORD dwRet = 0;
	Thread::ThreadInitParam_T* ptip = ( Thread::ThreadInitParam_T* )pParam;
	rendering_thread* pThread = ( rendering_thread* )( ptip->pThread );
	pThread->Init((( HWND )ptip->pUserParam ) );
	dwRet = pThread->Loop();
	pThread->Uninit();
	return dwRet;
}

rendering_thread::rendering_thread( void )
		: m_renderdevice( NULL ), m_frame_cnt( 0 )
{
	SetThreadProcAddress( RenderingThreadMain );
}

rendering_thread::~rendering_thread( void )
{
	//Uninit();//delete m_renderdevice;
}

BOOL rendering_thread::Init( HWND hWnd )
{
	if ( m_renderdevice != NULL )
		return TRUE;
	renderdevice_plugin* plugin = ( renderdevice_plugin* )( get_core()->get_plugin( plugintype_renderer ) );
	m_renderdevice = plugin->create_renderer();
	m_renderdevice->SetHostThread( this );
	renderer_create_param p;
	p.backbuffer_count = 1;
	p.w = 1024;
	p.h = 768;
	p.host_window = hWnd;
	p.windowed = TRUE;
	m_renderdevice->Create( p );
	return TRUE;
}

BOOL rendering_thread::Uninit()
{
	if ( m_renderdevice != NULL )
	{
		m_renderdevice->Destroy();
		m_renderdevice = NULL;
	}
	return TRUE;
}

DWORD rendering_thread::Loop()
{
	int framecnt = 0;
	while ( 1 )
	{
		DWORD dwRet = WaitForSingleObject( GetThreadTermEvent(), 0 );
		get_core()->render_thread_lock.reset();
		get_core()->main_thread_lock.set();
		if ( dwRet != WAIT_OBJECT_0 )
			get_core()->render_thread_lock.wait();
		if ( dwRet == WAIT_ABANDONED )
		{
			get_core()->get_log()->write( 1, 0xff0000, _T( "rendering thread ended abnormally!" ) );
			return -1;
		}
		if ( m_rendertask_queue.execute_all() != grc_succeeded )
			return -2;
		if ( dwRet == WAIT_OBJECT_0 )
		{
			get_core()->get_log()->write( 1, 0x7f7f7f, _T( "rendering thread ended normally." ) );
			return 0;
		}
		next_frame();
	}
}

void rendering_thread::next_frame()
{
	//get_core()->get_timer()->update();
	//double time = get_core()->get_timer()->get_elapsed();
	//get_core()->get_frame_synchronizer()->done_and_wait( m_renderdevice_sync );
	////m_renderdevice_sync.work_start();
	//get_core()->get_log()->write( 0, 0x000000ff, _T("[render thread]: end frame: %d\n"), m_frame_cnt );
	//m_frame_cnt++;
	//get_core()->get_log()->write( 0, 0x00000ff, _T("[render thread]: start frame: %d \n"), m_frame_cnt );
}

BOOL rendering_thread::flush_all_task()
{
	event event_done;
	event_done.create( FALSE, FALSE, 0 );
	return event_done.wait();
}
void rendering_thread::OnBegin()
{
}

void rendering_thread::OnEnd()
{
}

void rendering_thread::OnBeginKill()
{
	get_core()->render_thread_lock.set();
}

//Tasks
BOOL rendering_thread::AddRenderTask( task* pTask )
{
	get_core()->get_log()->write( 0, 0x000000ff, _T( "Add Task\n" ) );
	if ( get_core()->renderingthread_disabled() )
	{
		pTask->execute();
		delete pTask;
	}
	else
		m_rendertask_queue.add_task( pTask );
	return TRUE;
}
}