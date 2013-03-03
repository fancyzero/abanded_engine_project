#include "stdafx.h"
#include "utility.h"

namespace mania
{
static BOOL g_stringtable_initialized = FALSE;
static std::map<string, string>	g_stringtable;

const string& get_string( const string& idstring )
{
	if ( !g_stringtable_initialized )
		init_stringtable( _T( ".\\language\\en" ) );
	static std::map<string, string>::iterator it = g_stringtable.find( idstring );
	if ( it != g_stringtable.end() )
		return it->second;
	else
		return g_stringtable[_T( "ID_STRING_NOT_FOUND" )];
}

DWORD init_stringtable( const string& filename )
{
	FILE* fp = NULL;
	g_stringtable_initialized = TRUE;
	g_stringtable[_T( "ID_STRING_NOT_FOUND" )] = _T( "{string not found}" );
	errno_t errn = _wfopen_s( &fp, filename.c_str(), _T( "r" ) );
	if ( errn == 0 )
	{
		while ( !feof( fp ) )
		{
			TCHAR wszline[2048];
			string line = fgetws( wszline, 2048, fp );
			if ( line.empty() )
				continue;
			string::size_type pos = line.find_first_of( _T( '=' ) );
			string idpart = line.substr( 0, pos );
			string textpart = line.substr( pos );
		}
		return grc_succeeded;
	}
	else
		return grc_open_file_error;
}

MANIA_BASIS_API string convert_to_xmlurl( const string& url )
{
	//get rootdir
	string::size_type pos = url.find_first_of( _T( '\\' ) );
	if ( pos == string::npos )
		//error!
		return url;
	string root_dir = url.substr( 0, pos );
	string path = url.substr( pos );
	string ret = root_dir + _T( ".xml" ) + path;
	return ret;
}
}