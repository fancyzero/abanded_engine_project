#include "StdAfx.h"
#include "..\common.h"
#include "synchronizer.h"

namespace mania
{
synchronizer::synchronizer( void )
{
	m_event_more_work.create( FALSE, FALSE, NULL );
}

synchronizer::~synchronizer( void )
{
	//do_wait();
}

DWORD synchronizer::more_work()
{
	get_core()->get_log()->write( 0, 0x7f7f7f, _T( "dowait +++\n" ) );
	std::vector<synchronization*>::iterator it;
	for ( it = m_synchronizations.begin(); it != m_synchronizations.end(); ++it )
	{
		( *it )->reset();
	}
	get_core()->get_log()->write( 0, 0x7f7f7f, _T( "dowait ---\n" ) );
	m_event_more_work.set();
	m_event_more_work.reset();
	return grc_succeeded;
}

DWORD synchronizer::done_and_wait( synchronization& sync )
{
	HANDLE handle = sync.get_handle();
	::SignalObjectAndWait( handle, m_event_more_work.get_handle(), -1, FALSE );
	return grc_succeeded;
}

DWORD synchronizer::do_wait()
{
	get_core()->get_log()->write( 0, 0x7f7f7f, _T( "dowait +++\n" ) );
	std::vector<synchronization*>::iterator it;
	for ( it = m_synchronizations.begin(); it != m_synchronizations.end(); ++it )
	{
		( *it )->wait_and_reset();
	}
	get_core()->get_log()->write( 0, 0x7f7f7f, _T( "dowait ---\n" ) );
	return grc_succeeded;
}


void synchronizer::end()
{
	m_event_more_work.set();
}

DWORD synchronizer::init()
{
	return grc_succeeded;
}

void synchronizer::add_synchronization( synchronization* elem )
{
	if ( std::find( m_synchronizations.begin(), m_synchronizations.end(), elem ) == m_synchronizations.end() )
		m_synchronizations.push_back( elem );
}

void synchronizer::remove_synchronization( synchronization* elem )
{
	std::vector<synchronization*>::iterator it;
	it = std::find( m_synchronizations.begin(), m_synchronizations.end(), elem );
	if ( it != m_synchronizations.end() )
		m_synchronizations.erase( it );
}
}