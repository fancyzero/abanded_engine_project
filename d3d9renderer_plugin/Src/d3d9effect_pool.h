#pragma once

namespace mania
{
	class d3d9effect_pool :public effect_pool
	{
	public:
	d3d9effect_pool( renderdevice_ptr renderdevice );
	~d3d9effect_pool();

	virtual DWORD set_global_param( const char* name, const math::matrix44& mat )		;
	virtual DWORD set_global_param( const char* name, float value )						;						
	virtual DWORD set_global_param( const char* name, const float value[], UINT count )	;	
	virtual DWORD set_global_param( const char* name, int value )						;	
	virtual DWORD set_global_param( const char* name, const int value[], UINT count )	;	
	virtual DWORD set_global_param( const char* name, const sptr<texture> tex )			;	

	};

}