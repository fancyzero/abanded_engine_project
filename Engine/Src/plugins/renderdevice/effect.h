#pragma once

namespace mania
{
class effect : public render_resource
{
public:
	effect( renderdevice_ptr renderdevice )
			: render_resource( renderdevice )
	{
	}
	virtual DWORD create( const string& url ) = 0;
	virtual DWORD destroy() = 0;
	virtual DWORD begin( UINT& passes, DWORD flags ) = 0;
	virtual DWORD end() = 0;
	virtual DWORD begin_pass( UINT pass ) = 0;
	virtual DWORD end_pass() = 0;

	virtual DWORD set_param( const char* name, const math::matrix44& mat )			= 0;
	virtual DWORD set_param( const char* name, const math::matrix44 mats[], UINT count )	= 0;
	virtual DWORD set_param( const char* name, float value )						= 0;
	virtual DWORD set_param( const char* name, const float value[], UINT count )	= 0;
	virtual DWORD set_param( const char* name, int value )							= 0;
	virtual DWORD set_param( const char* name, const int value[], UINT count )		= 0;
	virtual DWORD set_param( const char* name, const sptr<texture> tex )			= 0;
};
}