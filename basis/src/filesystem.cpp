#include "stdafx.h"
#include "common.h"
#include "filesystem.h"

MANIA_NAMESPACE_BEGIN
namespace filesystem
{
file::file()
		: m_handle( NULL )
{
}
file::~file()
{
	if ( m_handle )
		fclose( m_handle );
}
DWORD file::open( const string& filename, DWORD mode )
{
	string modestr;
	if ( mode & fileopen_read )
		modestr += _T( "r" );
	if ( mode & fileopen_write )
		modestr += _T( "w" );
	if ( mode & fileopen_binary )
		modestr += _T( "b" );
	if ( mode & fileopen_unicode )
		modestr += _T( ",ccs=UNICODE" );
	errno_t errn = _wfopen_s( &m_handle, filename.c_str(), modestr.c_str() );
	if ( errn == 0 )
		return grc_succeeded;
	else
		return grc_open_file_error;
}
DWORD file::read_line( string& line )
{
	if ( !valid() )
		return grc_failed;
	TCHAR text[2048];
	fgetws( text, 2048, m_handle );
	line = text;
	return grc_succeeded;
}
size_t file::read( BYTE* dest, unsigned dest_size, unsigned int len )
{
	if ( !valid() )
		return grc_failed;
	return fread_s( dest, dest_size, len, 1, m_handle );
}
DWORD file::seek( long offset, seek_origin org )
{
	if ( !valid() )
		return grc_failed;
	int origin = 0;
	switch ( org )
	{
	case seek_set:
		origin = SEEK_SET;
		break;
	case seek_cur:
		origin = SEEK_CUR;
		break;
	case seek_end:
		origin = SEEK_END;
		break;
	default:
		return grc_failed;
	}
	if ( fseek( m_handle, offset, origin ) == 0 )
		return grc_succeeded;
	else
		return grc_failed;
}
BOOL  file::eof()
{
	if ( m_handle )
		return feof( m_handle );
	else
		return TRUE;
}

unsigned int file::get_length()
{
	unsigned int currpos = ( unsigned int )ftell( m_handle );
	seek( 0, seek_end );
	unsigned int ret = ( unsigned int )ftell( m_handle );
	seek( currpos, seek_set );
	return ret;
}

}
MANIA_NAMESPACE_END