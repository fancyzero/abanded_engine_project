#include "stdafx.h"
#include "Common.h"
#include "Log.h"

namespace mania
{

log::log( void )
{
	//AllocConsole();
	//SetConsoleTitle(_T("mania::log"));
	//m_output_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	//m_input_handle = GetStdHandle(STD_INPUT_HANDLE);
	InitializeCriticalSection( &m_cs );
	TCHAR filename[MAX_PATH];
	SYSTEMTIME systime;
	::GetLocalTime( &systime );
	::_stprintf_s( filename, MAX_PATH, _T( ".\\log\\log_%d-%d-%d_%d-%d-%d.htm" ),
	               systime.wYear, systime.wMonth, systime.wDay,
	               systime.wHour, systime.wMinute, systime.wSecond );
	CreateDirectory( _T( ".\\log" ), NULL );
	errno_t err = ::_wfopen_s( &m_file, filename, _T( "a" ) );
	if ( err != 0 )
		m_file = NULL;
	::fwprintf_s( m_file, _T( "<html>" ) );
	write( 1000, 0xffffff, _T( "Log started at: %d-%d-%d %d:%d:%d\n" ), systime.wYear, systime.wMonth, systime.wDay,
	       systime.wHour, systime.wMinute, systime.wSecond );
}

log::~log( void )
{
	if ( m_file )
	{
		::fwprintf_s( m_file, _T( "</html>" ) );
		fclose( m_file );
	}
}

log* log::m_instance = NULL;

log* log::get_instance()
{
	if ( m_instance == NULL )
		m_instance = new log();
	return m_instance;
}


void log::write( int level, DWORD color, const TCHAR* format, ... )
{
	if ( level < 1 )
		return;
	if ( m_file == NULL )
		return;
	const int DESTBUF_SIZE = 40960;
	TCHAR destbuf[DESTBUF_SIZE];
	TCHAR destbuf2[DESTBUF_SIZE];
	va_list va;
	va_start( va, format );
	vswprintf_s( destbuf, DESTBUF_SIZE, format, va );
	va_end( va );
	DWORD written = 0;
	written;
	::_stprintf_s( destbuf2, DESTBUF_SIZE, _T( "<font color=\"%x\"> %s </font><br>" ), color, &destbuf );
	DWORD r, g, b;
	WORD texattr = 0;
	r = ( color & 0xff0000 ) >> 16;
	g = ( color & 0xff00 ) >> 8;
	b = ( color & 0xff );
	if ( r > 128 || b > 128 || g > 128 )
		texattr = FOREGROUND_INTENSITY;
	if ( r > 85 )
		texattr |= FOREGROUND_RED;
	if ( g > 85 )
		texattr |= FOREGROUND_GREEN;
	if ( b > 85 )
		texattr |= FOREGROUND_BLUE;
	EnterCriticalSection( &m_cs );
	wprintf( destbuf );
	//::SetConsoleTextAttribute( m_output_handle, texattr );
	//::WriteConsole( m_output_handle, (void*)destbuf, (DWORD)(wcslen( destbuf )), &written, 0 );
	::fwprintf_s( m_file, destbuf2 );
	LeaveCriticalSection( &m_cs );
}
//void log::clear_console()
//{
//	COORD coord_size,coord_pos;
//	coord_pos.X = 0;
//	coord_pos.Y = 0;
//	coord_size.X = 100;
//	coord_size.Y = 1024;
//	SetConsoleScreenBufferSize( m_output_handle, coord_size );
//	DWORD n;
//	FillConsoleOutputCharacter( m_output_handle, _T(' '), coord_size.X * coord_size.Y, coord_pos, &n );
//	SetConsoleCursorPosition( m_output_handle, coord_pos );
//}


log* get_log()
{
	return log::get_instance();
}
}