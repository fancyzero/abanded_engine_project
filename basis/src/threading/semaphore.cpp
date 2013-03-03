#include "StdAfx.h"
#include "semaphore.h"

namespace mania
{
semaphore::semaphore( void )
		: m_handle( NULL )
{
}

semaphore::~semaphore( void )
{
}



BOOL semaphore::create( LONG initCount, LONG maxCount, const TCHAR* name )
{
	_ASSERT( m_handle == NULL );
	m_handle = CreateSemaphore( NULL, initCount, maxCount, name );
	return m_handle != NULL;
}

BOOL semaphore::wait( DWORD dwWaitTime )
{
	if ( m_handle != NULL )
	{
		if ( WaitForSingleObject( m_handle, dwWaitTime ) == WAIT_OBJECT_0 )
		{
			return TRUE;
		}
		else
			return FALSE;
	}
	return FALSE;
}

BOOL semaphore::release()
{
	LONG prevCount;
	return ReleaseSemaphore( m_handle, 1, &prevCount );
}
}