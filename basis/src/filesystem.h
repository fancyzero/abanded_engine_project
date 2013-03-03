#pragma once

namespace mania
{
namespace filesystem
{
enum efileopen_mode
{
	fileopen_read		= 1,
	fileopen_write		= 1 << 1,
	fileopen_binary		= 1 << 2,
	fileopen_unicode	= 1 << 3,
};

enum seek_origin
{
	seek_set,
	seek_cur,
	seek_end
};

class MANIA_BASIS_API file
{
public:
	file();
	~file();
	DWORD open( const string& filename, DWORD mode );
	DWORD read_line( string& line );
	size_t read( BYTE* dest, unsigned dest_size, unsigned int len );
	DWORD seek( long offset, seek_origin org );
	BOOL  eof();
	unsigned int get_length();

	BOOL  valid() { return m_handle != NULL; }
	const string& get_filename() { return m_filename; }
private:
	string 	m_filename;
	FILE*	m_handle;
};
}
}