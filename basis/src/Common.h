#pragma once
#pragma warning (disable: 4251 )
#define WINVER		0x0500
#include <windows.h>
#include <tchar.h>
#include <vector>
#include <stack>
#include <list>
#include <string>
#include <map>


#define MANIA_NAMESPACE_BEGIN namespace mania {
#define MANIA_NAMESPACE_END }

#define null NULL

#ifdef DLL_EXPORT
#define EXPIMP_TEMPLATE
#else
#define EXPIMP_TEMPLATE extern
#endif

#ifdef FOR_SWIG
#define MANIA_BASIS_API
#else
	#ifdef BASIS_DLL_EXPORT
		#define MANIA_BASIS_API __declspec(dllexport)
	#else
		#define MANIA_BASIS_API __declspec(dllimport)
	#endif
#endif

MANIA_NAMESPACE_BEGIN
#if defined UNICODE
typedef std::wstring string;
#else
typedef std::string string;
#endif

enum GeneralReturnCode
{
	grc_succeeded	= 0,
	grc_failed		= -1,
	grc_url_not_found = -2,
	grc_open_file_error = -3,
};

MANIA_NAMESPACE_END

#include "math\math.h"
#include "basic\classinfo.h"
#include "basic\ref_counted.h"
#include "utility.h"
#include "basic\sptr.h"
#include "threading\ThreadingBase.h"
#include "basic\object.h"

MANIA_NAMESPACE_BEGIN
typedef DWORD gresult;
#define succeeded( x ) ( x >= 0 )
#define failed( x ) ( (int)x < 0 )
#define safe_Release( x ) if ( x ) x->Release(); x = NULL;
#define safe_release( x ) if ( x ) x->release(); x = NULL;
#define safe_delete( x ) if ( x ) delete x; x = NULL;
#define safe_delete_array( x ) if ( x ) delete []x; x = NULL;
#define foreachc( c, v, l ) for( c::const_iterator v = l.begin();  v != l.end(); v++ )
#define foreach( c, v, l ) for( c::iterator v = l.begin();  v != l.end(); v++ )
#define return_if_failed(gr) if ( failed(gr) ) return gr;
MANIA_NAMESPACE_END

#define check(x) _ASSERT(x)

