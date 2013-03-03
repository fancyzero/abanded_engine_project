#include "stdafx.h"
#include "..\common.h"
#include "ThreadingBase.h"
#include "taskqueuemanager.h"



namespace mania
{
taskqueue_manager::taskqueue_manager( void )
{
	m_operate_lock.create( NULL );
	m_queue = new std::queue<task*>();
}

taskqueue_manager::~taskqueue_manager( void )
{
	delete m_queue;
}

DWORD taskqueue_manager::add_task( task* pTask )
{
	if ( m_operate_lock.lock() == TRUE )
	{
		m_queue->push( pTask );
		m_operate_lock.unlock();
		return grc_succeeded;
	}
	return grc_failed;
}

BOOL taskqueue_manager::is_empty()
{
	return m_queue->empty();
}

DWORD taskqueue_manager::execute_all()
{
	while ( 1 )
	{
		if ( m_operate_lock.lock() == TRUE )
		{
			if ( m_queue->size() <= 0 )
			{
				m_operate_lock.unlock();
				break;
			}
			task* pTask = m_queue->front();
			m_queue->pop();
			m_operate_lock.unlock();
			if ( pTask == NULL )
				continue;
			else
			{
				pTask->execute();
				//get_core()->get_log()->write( 0, 0xff03ff, _T("Just executed an task.\n"));
				delete pTask;
			}
		}
		else
		{
			return grc_failed;
		}
	}
	return grc_succeeded;
}
}