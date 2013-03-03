#include "stdafx.h"
#include "..\common.h"
#include "Thread.h"

namespace mania
{

DWORD Thread::default_proc( void* param )
{
	ThreadInitParam_T* tip = ( ThreadInitParam_T* )param;
	if ( succeeded( tip->pThread->OnBegin( tip->pUserParam ) ) )
	{
		while ( 1 )
		{
			DWORD dwRet = WaitForSingleObject( tip->pThread->GetThreadTermEvent(), 0 );
			if ( dwRet == WAIT_OBJECT_0 )
				return grc_succeeded;
			gresult gret = tip->pThread->OnLoop( tip->pUserParam );
			if failed( gret )
				return gret;
		}
	}
	return grc_failed;
}

Thread::Thread( void )
		: m_dwThreadID( 0 ), m_hThreadHandle( 0 ), m_hThreadTermEvent( 0 ),
		m_pThreadParam( NULL )
{
	m_pTreadProcAddress = (LPTHREAD_START_ROUTINE)default_proc;
}

Thread::~Thread( void )
{
	if ( m_hThreadTermEvent != NULL )
	{
		KillSoftly( 5000 );
		::CloseHandle( m_hThreadTermEvent );
		m_hThreadTermEvent = NULL;
	}
}

BOOL Thread::Begin( void* pParam, BOOL bStartSuspended )
{
	m_pThreadParam = pParam;
	if ( m_pTreadProcAddress == NULL )
		return FALSE;
	ThreadInitParam_T* tip;
	tip = new ThreadInitParam_T;
	tip->pThread = this;
	tip->pUserParam = pParam;
	m_hThreadTermEvent = ::CreateEvent( NULL, TRUE, FALSE, NULL );
	if ( m_hThreadTermEvent == NULL )
		return FALSE;
	if ( bStartSuspended == TRUE )
		m_hThreadHandle = CreateThread( NULL, 0, m_pTreadProcAddress, tip, CREATE_SUSPENDED, &m_dwThreadID );
	else
		m_hThreadHandle = CreateThread( NULL, 0, m_pTreadProcAddress, tip, 0, &m_dwThreadID );
	if ( m_hThreadHandle != NULL )
		return TRUE;
	else
		return FALSE;
}

BOOL Thread::Pause()
{
	DWORD dwRet = SuspendThread( m_hThreadHandle );
	if ( dwRet == 0xffffffff )
		return FALSE;
	else
		return TRUE;
}

BOOL Thread::Resume()
{
	DWORD dwRet = ResumeThread( m_hThreadHandle );
	if ( dwRet == 0xffffffff )
		return FALSE;
	else
		return TRUE;
}

BOOL Thread::KillSoftly( DWORD dwTime )
{
	if ( m_hThreadTermEvent != NULL )
	{
		::SetEvent( m_hThreadTermEvent );
		OnBeginKill();
		if ( WaitForSingleObject( m_hThreadHandle, dwTime ) == WAIT_TIMEOUT )
		{
			::TerminateThread( m_hThreadHandle, -1 );
			return FALSE;
		}
		::CloseHandle( m_hThreadTermEvent );
		m_hThreadTermEvent = NULL;
		OnEnd( m_pThreadParam );
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL Thread::Kill( DWORD dwCode )
{
	::TerminateThread( m_hThreadHandle, dwCode );
	return TRUE;
}

gresult Thread::wait( DWORD mseconds )
{
	DWORD ret = WaitForSingleObject( m_hThreadHandle, mseconds );
	if ( ret == WAIT_OBJECT_0 )
		return grc_succeeded;
	else
		return grc_failed;
}
}