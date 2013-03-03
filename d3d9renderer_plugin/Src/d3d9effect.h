#pragma once

namespace mania
{

class d3d9renderdevice;
class d3d9effect;
typedef resource_object< d3d9effect*, ID3DXEffect*> d3d9effect_res_base;
class d3d9effect_res : public d3d9effect_res_base
{
	friend class d3d9effect;
public:
	d3d9effect_res( d3d9effect* host )
			: resource_object( host )
	{
	}
public:
	DWORD create( const string& url );
	DWORD destroy();
};

class d3d9effect : public effect
{
public:
	d3d9effect( d3d9renderer_ptr renderdevice );
	~d3d9effect( void );

	virtual DWORD create( const string& url );
	virtual DWORD destroy();
	virtual DWORD begin( UINT& passes, DWORD flags );
	virtual DWORD end();
	virtual DWORD begin_pass( UINT pass );
	virtual DWORD end_pass();
	virtual DWORD on_lost_device();
	virtual DWORD on_reset_device();

	virtual DWORD set_param( const char* name, const math::matrix44& mat );
	virtual DWORD set_param( const char* name, const math::matrix44 mats[], UINT count );
	virtual DWORD set_param( const char* name, float value );
	virtual DWORD set_param( const char* name, const float value[], UINT count );
	virtual DWORD set_param( const char* name, int value );
	virtual DWORD set_param( const char* name, const int value[], UINT count );
	virtual DWORD set_param( const char* name, const sptr<texture> tex );
protected:
	sptr<d3d9effect_res>	m_effectres;
	d3d9renderer_ptr		m_renderdevice;
};

}