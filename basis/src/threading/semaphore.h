#pragma once



namespace mania
{
class semaphore
{
public:
	semaphore( void );
	~semaphore( void );

	BOOL create( LONG initCount, LONG maxCount, const TCHAR* name );
	BOOL wait( DWORD dwWaitTime = INFINITE );
	BOOL release();
private:
	HANDLE		m_handle;
};

}