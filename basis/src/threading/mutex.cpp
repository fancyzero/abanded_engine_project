#include "stdafx.h"
#include "..\common.h"
#include "mutex.h"

namespace mania
{
mutex::mutex( void )
		: m_hMutex( NULL )
{
}

mutex::~mutex( void )
{
}

BOOL mutex::create( BOOL bInitOwner, TCHAR* ptszName )
{
	m_hMutex = CreateMutex( NULL, bInitOwner, ptszName );
	if ( m_hMutex == NULL )
		return FALSE;
	return TRUE;
}

BOOL mutex::wait( DWORD dwWaitTime )
{
	if ( m_hMutex != NULL )
	{
		if ( WaitForSingleObject( m_hMutex, dwWaitTime ) == WAIT_OBJECT_0 )
		{
			return TRUE;
		}
		else
			return FALSE;
	}
	return FALSE;
}

BOOL mutex::release()
{
	return ReleaseMutex( m_hMutex );
}
}