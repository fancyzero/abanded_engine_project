#pragma once

namespace mania
{
class task;
class MANIA_BASIS_API taskqueue_manager
{
public:
	taskqueue_manager( void );
	virtual ~taskqueue_manager( void );

	DWORD add_task( task* pTask );
	DWORD execute_all();
	BOOL  is_empty();

private:

	critical_section	m_operate_lock;
	std::queue<task*>*	m_queue;
};

}