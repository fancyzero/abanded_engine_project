#pragma once

namespace mania
{
class MANIA_BASIS_API critical_section
{
public:
	critical_section( void );
	~critical_section( void );

	BOOL lock();
	BOOL unlock();
	BOOL create( TCHAR* ptszName );
	BOOL initialized();
private:
	BOOL m_initialized;
	CRITICAL_SECTION	m_cs;
};
}