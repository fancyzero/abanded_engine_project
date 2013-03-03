#include "StdAfx.h"
#include "d3d9effect_pool.h"

namespace mania
{
d3d9effect_pool::d3d9effect_pool( renderdevice_ptr renderdevice )
		: effect_pool( renderdevice )
{
}

d3d9effect_pool::~d3d9effect_pool( void )
{
}


DWORD d3d9effect_pool::set_global_param( const char* name, const math::matrix44& mat )
{
	return  0;
}

DWORD d3d9effect_pool::set_global_param( const char* name, float value )
{
	return  0;
}

DWORD d3d9effect_pool::set_global_param( const char* name, const float value[], UINT count )
{
	return  0;
}

DWORD d3d9effect_pool::set_global_param( const char* name, int value )
{
	return  0;
}

DWORD d3d9effect_pool::set_global_param( const char* name, const int value[], UINT count )
{
	return  0;
}

DWORD d3d9effect_pool::set_global_param( const char* name, const sptr<texture> tex )
{
	return  0;
}

}