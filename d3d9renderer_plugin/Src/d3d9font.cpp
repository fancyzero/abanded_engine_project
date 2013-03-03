#include "StdAfx.h"
#include "d3d9renderdevice.h"
#include "d3d9font.h"

namespace mania
{

MANIA_IMPLEMENT_CLASS( d3dfont_res_base );

DWORD d3d9font_res::create( const string& url )
{
	FILE* fp = NULL;
	errno_t errn = _wfopen_s( &fp, url.c_str(), _T( "r" ) );
	if ( errn == 0 )
	{
		return grc_succeeded;
	}
	else
		return grc_url_not_found;
}

DWORD d3d9font_res::destroy()
{
	safe_Release( m_object );
	return grc_succeeded;
}

d3d9font::~d3d9font( void ) {}


DWORD d3d9font::create( const string& url )
{
	m_d3dfont_res = new d3d9font_res( this );
	m_d3dfont_res->create( url );
	return grc_succeeded;
}

DWORD d3d9font::destroy()
{
	return grc_succeeded;
}
}