#pragma once

namespace mania
{
class MANIA_BASIS_API event
{
public:
	event( void );
	~event( void );

	int wait( DWORD dwWaitTime = INFINITE );
	int msg_wait( DWORD wait_time = INFINITE );
	BOOL create( BOOL bManuReset, BOOL bInitState, TCHAR* ptszName );
	BOOL set();
	BOOL reset();
	HANDLE get_handle() { return m_handle; }
private:
	HANDLE	m_handle;
};
}