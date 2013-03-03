#pragma once

namespace mania
{
class synchronizer;

class synchronization
{
public:
	virtual BOOL wait_and_reset() = 0;
	virtual BOOL reset() = 0;
	virtual HANDLE get_handle() = 0;
};

class synchronizer
{
public:
	synchronizer( void );
	~synchronizer( void );

	DWORD more_work();
	DWORD done_and_wait( synchronization& sync );
	DWORD do_wait();
	void  end();
	DWORD init();

	void add_synchronization( synchronization* elem );
	void remove_synchronization( synchronization* elem );
private:
	event							m_event_more_work;
	std::vector<synchronization*>	m_synchronizations;
};
}