#pragma once

namespace mania
{
class mutex
{
public:
	mutex( void );
	~mutex( void );

	BOOL create( BOOL bInitOwner, TCHAR* ptszName );
	BOOL wait( DWORD dwWaitTime = INFINITE );
	BOOL release();
private:
	HANDLE		m_hMutex;
};
}