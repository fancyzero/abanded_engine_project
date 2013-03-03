#pragma once

namespace mania
{
class effect_pool : public object
{
public:
	MANIA_DECLAR_ABSTRACTL_CLASS(effect_pool);
public:
	effect_pool( renderdevice_ptr renderdevice )
		:m_renderdevice(renderdevice)
	{
	}
	virtual ~effect_pool(){};

	virtual gresult create() = 0;
	virtual DWORD set_global_param( const char* name, const math::matrix44& mat )		=0;
	virtual DWORD set_global_param( const char* name, float value )						=0;						
	virtual DWORD set_global_param( const char* name, const float value[], UINT count )	=0;	
	virtual DWORD set_global_param( const char* name, int value )						=0;	
	virtual DWORD set_global_param( const char* name, const int value[], UINT count )	=0;	
	virtual DWORD set_global_param( const char* name, const sptr<texture> tex )			=0;	

protected:
	renderdevice_ptr m_renderdevice;
};
}