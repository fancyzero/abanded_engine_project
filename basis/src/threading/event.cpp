#include "stdafx.h"
#include "..\common.h"
#include "threadingbase.h"
#include "event.h"

namespace mania
{
event::event( void )
{
}

event::~event( void )
{
}

int event::wait( DWORD dwWaitTime )
{
	if ( m_handle != NULL )
	{
		DWORD ret = WaitForSingleObject( m_handle, dwWaitTime );
		if ( ret == WAIT_OBJECT_0 )
			return thread_wait_obj0;
		else if ( ret == WAIT_ABANDONED )
			return thread_wait_abandoned;
		else
			return thread_wait_timeout;
	}
	return thread_wait_failed;
}

int event::msg_wait( DWORD wait_time )
{
	if ( m_handle != NULL )
	{
		DWORD ret = ::MsgWaitForMultipleObjects( 1, &m_handle, FALSE, wait_time, QS_ALLEVENTS );
		if ( ret == WAIT_OBJECT_0 )
			return thread_wait_obj0;
		else if ( ret == WAIT_OBJECT_0 + 1 )
			return thread_wait_input;
		else if ( ret == WAIT_ABANDONED )
			return thread_wait_abandoned;
		else
			return thread_wait_timeout;
	}
	return thread_wait_failed;
}

BOOL event::create( BOOL bManuReset, BOOL bInitState, TCHAR* ptszName )
{
	m_handle = CreateEvent( NULL, bManuReset, bInitState, ptszName );
	if ( m_handle == NULL )
		return FALSE;
	return TRUE;
}

BOOL event::set()
{
	return SetEvent( m_handle );
}

BOOL event::reset()
{
	return ResetEvent( m_handle );
}
}