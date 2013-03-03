#pragma once

namespace mania
{

class MANIA_BASIS_API log
{
public:
	log( void );
	~log( void );

	void write( int level, DWORD color, const TCHAR* format, ... );
	static log* get_instance();
private:
	FILE*	m_file;
	CRITICAL_SECTION  m_cs;
	static log* m_instance;

};

MANIA_BASIS_API log* get_log();
}