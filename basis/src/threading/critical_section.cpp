#include "stdafx.h"
#include "..\common.h"
#include "critical_section.h"

namespace mania
{
critical_section::critical_section( void )
		: m_initialized( FALSE )
{
}

critical_section::~critical_section( void )
{
}

BOOL critical_section::initialized()
{
	return m_initialized;
}

BOOL critical_section::lock()
{
	EnterCriticalSection( &m_cs );
	return TRUE;
}

BOOL critical_section::unlock()
{
	LeaveCriticalSection( &m_cs );
	return TRUE;
}

BOOL critical_section::create( TCHAR* ptszName )
{
	ptszName;
	InitializeCriticalSection( &m_cs );
	m_initialized = TRUE;
	return TRUE;
}

}